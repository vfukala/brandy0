/**
 * display-area.hpp
 *
 * Author: Viktor Fukala
 * Created on 2020/12/25
 */
#ifndef DISPLAY_AREA_HPP
#define DISPLAY_AREA_HPP

#include <gtkmm/glarea.h>
#include <giomm/resource.h>
#include <epoxy/gl.h>

#include "glob.hpp"

#include "display-modes.hpp"
#include "sim-frame.hpp"
#include "simulator-params.hpp"
#include "vec2d.hpp"

namespace brandy0
{

struct LineSegment
{
	vec2d p0, p1;

	LineSegment(const vec2d& p0, const vec2d& p1) : p0(p0), p1(p1)
	{
	}
};

struct UniformLoc
{
	std::string name;
	GLuint *pos;

	UniformLoc(const std::string& name, GLuint *const pos) : name(name), pos(pos)
	{
	}
};

typedef std::vector<UniformLoc> UniformLocVec;

class DisplayArea : public Gtk::GLArea
{
private:
	Glib::RefPtr<Gdk::GLContext> glContext;

	const SimulatorParams *params;
	Grid<bool> solid;
	SimFrame *curFrame;

	GLuint glWhiteProgram = 0;
	GLuint glWhiteMat = 0;
	GLuint glPaintProgram = 0;
	GLuint glPaintMat = 0;
	GLuint glWhiteVao = 0;
	GLuint glWhiteVbo = 0;
	GLuint glPaintVao = 0;
	GLuint glPaintVbo = 0;

	uint32_t backDisplayMode = BACK_DISPLAY_MODE_DEFAULT;
	uint32_t frontDisplayMode = FRONT_DISPLAY_MODE_DEFAULT;

	void realize();
	void unrealize();
	bool render(const Glib::RefPtr<Gdk::GLContext>& context);

	void initBuffers();
	//GLuint loadProgram(const std::string& vshaderName, const std::string& fshaderName);
	void initShaders();

	Point to_poi(const double x, const double y);
	Point to_poi(const vec2d& v);
	vec2d to_coor(const uint32_t x, const uint32_t y);
	vec2d to_coor(const Point p);
	void computeMat(float *mat);

	void addStreamLine(std::vector<LineSegment>&, const vec2d&);
	void addArrow(std::vector<LineSegment>& segs, const vec2d& pos);
	void drawContent();


public:
	DisplayArea();
	void setParams(const SimulatorParams *const);
	void setCurFrame(const SimFrame& curFrame);
	void setBackDisplayMode(const uint32_t backMode);
	void setFrontDisplayMode(const uint32_t frontMode);
	void redraw();
};

}

#endif // DISPLAY_AREA_HPP
