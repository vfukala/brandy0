/**
 * bc-selector.hpp
 * 
 * Author: Viktor Fukala
 * Created on 2020/12/27
 */
#ifndef BC_SELECTOR_HPP
#define BC_SELECTOR_HPP

#include <functional>

#include <gtkmm/comboboxtext.h>
#include <gtkmm/entry.h>
#include <gtkmm/frame.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>

#include "glob.hpp"

#include "annotated-entry.hpp"
#include "boundary-cond.hpp"
#include "func-utils.hpp"

namespace brandy0
{

class BCSelector : public Gtk::Frame
{
private:
    Gtk::Grid grid;
    Gtk::Label pressureLabel;
    Gtk::ComboBoxText pressureSelector;
    AnnotatedEntry uxEntry;
    AnnotatedEntry uyEntry;

    BoundaryCond bc;

	VoidFunc inputChangeHandler;

    void onPressureTypeChange();
    void setEntryFields();

public:
    PressureBoundaryCond selectedPressure;

    BCSelector(const std::string& atDescriptor, const VoidFunc& inputChangeHandler);//const std::function<void()>& validityChangeHandler);
    
    bool hasValidInput() const;
	BoundaryCond getBc() const;
	void setBc(const BoundaryCond& bc);
};

}

#endif // BC_SELECTOR_HPP