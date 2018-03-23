#include "class.h"
#include <map>

namespace Echo
{
	static std::map<String, ObjectFactory*>*	g_classInfos = nullptr;

	// add class
	void Class::addClass(const String& className, ObjectFactory* objFactory)
	{
		if (!g_classInfos)
		{
			g_classInfos = new std::map<String, ObjectFactory*>();
		}

		(*g_classInfos)[className] = objFactory;
	}

	// get all child class
	bool Class::getChildClasses(StringArray& childClasses, const char* className)
	{
		// keep clean
		childClasses.clear();

		if (!g_classInfos)
			return false;

		for (auto it : *g_classInfos)
		{
			if (it.second->m_parent == className)
			{
				childClasses.push_back(it.first);
			}
		}

		return !childClasses.empty();
	}

	Object* Class::create(const char* className)
	{
		auto it = g_classInfos->find(className);
		if (it != g_classInfos->end())
		{
			Object* obj = it->second->create();
			if (obj)
				obj->setName(className);

			return obj;
		}

		return nullptr;
	}
}