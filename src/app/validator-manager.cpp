/**
 * validator-manager.cpp
 * 
 * Author: Viktor Fukala
 * Created on 2021/1/16
 */
#include "validator-manager.hpp"

namespace brandy0
{

void ValidatorManager::plug(const BoolFunc& validator)
{
	validators.push_back(validator);
}

bool ValidatorManager::isAllValid() const
{
	for (const BoolFunc& v : validators)
		if (!v())
			return false;
	return true;
}

}