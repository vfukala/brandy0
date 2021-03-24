/**
 * shape-config-window.cpp
 * 
 * Author: Viktor Fukala
 * Created on 2021/01/14
 */
#include "shape-config-window.hpp"

#include "conv-utils.hpp"
#include "simulation-params-preset.hpp"

namespace brandy0
{

void ShapeConfigWindow::updateGeneralSensitivity()
{
	undoButton.set_sensitive(parent->params->shapeStack.canUndo());
	redoButton.set_sensitive(parent->params->shapeStack.canRedo());
	clearAllButton.set_sensitive(!parent->params->shapeStack.empty());
}

void ShapeConfigWindow::setEntryFields()
{
	uptr<SimulationParams>& params = parent->params;
	widthEntry.setText(ConvUtils::defaultToString(params->w));
	heightEntry.setText(ConvUtils::defaultToString(params->h));
}

void ShapeConfigWindow::updateAddShapeWidgets()
{
	if (addShapeMode == ADD_SHAPE_POLYGON)
	{
		polygonVerticesLabel.pseudoShow();
		const uint32_t vertSet = nextShapeClicks.size();
		const str labelText = std::to_string(vertSet) + " " + (vertSet == 1 ? "vertex" : "vertices") + " set";
		polygonVerticesLabel.set_text(labelText);
		polygonPopVertexButton.pseudoShow();
		polygonPopVertexButton.set_sensitive(vertSet != 0);
		polygonFinishButton.pseudoShow();
		polygonFinishButton.set_sensitive(vertSet >= 3);
	}
	else
	{
		polygonVerticesLabel.pseudoHide();
		polygonPopVertexButton.pseudoHide();
		polygonFinishButton.pseudoHide();
	}

	addingShapeFrame.set_label("current " + ADD_SHAPE_MODES[addShapeMode].name);
	clearShapeButton.set_sensitive(!nextShapeClicks.empty());
}

ShapeConfigWindow::ShapeConfigWindow(ConfigStateAbstr *parent)
    : BrandyWindow(1280, 720),
	dimensionsFrame("physical dimensions"),
    widthEntry("width:", &parent->app->styleManager),
    heightEntry("height:", &parent->app->styleManager),
	addingShapeLabel("adding shape"),
	addingShapeFrame("current rectangle"),
	polygonVerticesLabel("0 vertices set"),
	clearShapeButton("clear"),
	polygonPopVertexButton("pop vertex"),
	polygonFinishButton("finish"),
	generalFrame("general controls"),
    undoButton("undo"),
    redoButton("redo"),
	clearAllButton("clear all (can't undo!)"),
	shapeWidget(parent, this),
	parent(parent)
{
	widthEntry.hookInputHandler([this, parent]
			{
			ConvUtils::updatePosRealIndicator(widthEntry, parent->params->w, SimulationParamsPreset::DefaultW, SimulationParamsPreset::MinW, SimulationParamsPreset::MaxW);
			parent->validityChangeListeners.invoke();
			parent->dimensionsChangeListeners.invoke();
			}
			);
	heightEntry.hookInputHandler([this, parent]
			{
			ConvUtils::updatePosRealIndicator(heightEntry, parent->params->h, SimulationParamsPreset::DefaultH, SimulationParamsPreset::MinH, SimulationParamsPreset::MaxH);
			parent->validityChangeListeners.invoke();
			parent->dimensionsChangeListeners.invoke();
			}
			);
	
	undoButton.signal_clicked().connect([parent]{
		parent->params->shapeStack.undo();
		parent->shapeStackChangeListeners.invoke();
	});
	redoButton.signal_clicked().connect([parent]{
		parent->params->shapeStack.redo();
		parent->shapeStackChangeListeners.invoke();
	});
	clearAllButton.signal_clicked().connect([parent]{
		parent->params->shapeStack.clear();
		parent->shapeStackChangeListeners.invoke();
	});
	
	parent->shapeStackChangeListeners.plug([this]{
		updateGeneralSensitivity();
	});
	parent->paramsOverwriteListeners.plug([this]{
		nextShapeClicks.clear();
		addShapeMode = ADD_SHAPE_MODE_DEFAULT;
		shapeSelector.set_active(ADD_SHAPE_MODE_DEFAULT);
		nextShapeChangeListeners.invoke();
		updateGeneralSensitivity();
		setEntryFields();
	});
	parent->inputValidators.plug([this]{
		return widthEntry.hasValidInput() && heightEntry.hasValidInput();
	});

	for (const AddShapeMode mode : ADD_SHAPE_MODES)
		shapeSelector.append(mode.name);
	shapeSelector.set_active(ADD_SHAPE_MODE_DEFAULT);
	shapeSelector.signal_changed().connect([this]
	{
		const uint32_t newMode = shapeSelector.get_active_row_number();
		if (newMode != addShapeMode)
		{
			nextShapeClicks.clear();
			addShapeMode = newMode;
			nextShapeChangeListeners.invoke();
		}
	});

	nextShapeChangeListeners.plug([this]
	{
		updateAddShapeWidgets();
	});

	clearShapeButton.signal_clicked().connect([this]
	{
		nextShapeClicks.clear();
		nextShapeChangeListeners.invoke();
	});
	polygonPopVertexButton.signal_clicked().connect([this, parent]
	{
		if (addShapeMode == ADD_SHAPE_POLYGON && !nextShapeClicks.empty())
		{
			nextShapeClicks.pop_back();
			nextShapeChangeListeners.invoke();
		}
	});
	polygonFinishButton.signal_clicked().connect([this, parent]
	{
		if (addShapeMode == ADD_SHAPE_POLYGON && nextShapeClicks.size() >= 3)
		{
			parent->params->shapeStack.push(make_shared<ObstaclePolygon>(false, nextShapeClicks));
			nextShapeClicks.clear();
			parent->shapeStackChangeListeners.invoke();
			nextShapeChangeListeners.invoke();
		}
	});

	signal_delete_event().connect([parent](GdkEventAny*)
	{
		parent->shapeConfigOpened = false;
		parent->shapeConfigOpenedChangeListeners.invoke();
		return false;
	});

    widthEntry.attachTo(dimensionsGrid, 0, 0);
    heightEntry.attachTo(dimensionsGrid, 0, 1);
	dimensionsFrame.add(dimensionsGrid);
	rootGrid.attach(dimensionsFrame, 0, 0, 1, 2);

	rootGrid.attach(addingShapeLabel, 1, 0);
	rootGrid.attach(shapeSelector, 2, 0);

	addingShapeGrid.attach(polygonVerticesLabel, 0, 0, 2, 1);
	addingShapeGrid.attach(clearShapeButton, 0, 1);
	addingShapeGrid.attach(polygonPopVertexButton, 1, 1);
	addingShapeGrid.attach(polygonFinishButton, 0, 2, 2, 1);
	addingShapeFrame.add(addingShapeGrid);
	rootGrid.attach(addingShapeFrame, 1, 1, 2, 1);

	generalGrid.attach(undoButton, 0, 0);
	generalGrid.attach(redoButton, 0, 1);
	generalGrid.attach(clearAllButton, 0, 2);
	generalFrame.add(generalGrid);
	rootGrid.attach(generalFrame, 3, 0, 1, 2);

	rootGrid.attach(shapeWidget, 0, 2, 4, 1);
	shapeWidget.show();
    
    add(rootGrid);

    show_all_children();
}

}