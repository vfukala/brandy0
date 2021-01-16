/**
 * listener-manager.hpp
 * 
 * Author: Viktor Fukala
 * Created on 2021/1/16
 */
#ifndef LISTENER_MANAGER_HPP
#define LISTENER_MANAGER_HPP

#include <vector>

#include "func-utils.hpp"

namespace brandy0
{

class ListenerManager
{
private:
	std::vector<VoidFunc> listeners;

public:
	void plug(const VoidFunc& listener);
	void invoke() const;
};

}

#endif // LISTENER_MANAGER_HPP