/**
 * validity-indicator.hpp
 *
 * Author: Viktor Fukala
 * Created on 2020/12/4
 */
#ifndef VALIDITY_INDICATOR_HPP
#define VALIDITY_INDICATOR_HPP

#include <gtkmm/cssprovider.h>
#include <gtkmm/grid.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>

#include "glob.hpp"

namespace brandy0
{

enum IndicatorState
{
	OK, DEFAULT, INVALID
};

class ValidityIndicator : public Gtk::Grid
{
private:
	Gtk::Label message;
	Gtk::Image* icon;
	IndicatorState state;
	void setOkIcon();
	void setInvalidIcon();
	Glib::RefPtr<Gtk::CssProvider> currentStyle;
	void changeStyle(const Glib::RefPtr<Gtk::CssProvider> style);
public:
	ValidityIndicator();
	~ValidityIndicator() override;
	void setDefault();
	void setOk();
	void setInvalid(const std::string& message);
	IndicatorState getState() const;
};

}

#endif // VALIDITY_INDICATOR_HPP