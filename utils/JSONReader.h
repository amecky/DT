#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include "stringutils.h"
#include "..\stdafx.h"
#include "..\renderer\render_types.h"
#include "Log.h"
#include <Vector.h>
#include <assert.h>

// -------------------------------------------------------
// Category
// -------------------------------------------------------
class Category {

struct SectionEntry {
	IdString hash;
	std::string value;
	std::string name;
};

typedef std::vector<SectionEntry> Entries;

public:
	Category(const std::string& name) { 
		_name = string::murmur_hash(name.c_str());
	}

	~Category() {
		std::vector<Category*>::iterator it = _children.begin();
		while ( it != _children.end() ) {
			delete *(it);
			it = _children.erase(it);
		}
	}
	
	const IdString getName() const {
		return _name;
	}

	void addProperty(const std::string& name,const std::string& value) {
		SectionEntry entry;
		entry.hash = string::murmur_hash(name.c_str());
		entry.value = value;		
		entry.name = name;
		_entries.push_back(entry);
	}

	void getPropertyNames(std::vector<std::string>& propertyNames) {
		for ( size_t i = 0; i < _entries.size(); ++i ) {
			propertyNames.push_back(_entries[i].name);
		}
	}

	std::string getProperty(const std::string& name) {
		std::string ret;
		getProperty(name,ret);
		return ret;		
	}

	bool getProperty(const std::string& name,std::string& ret) {
		IdString hash = string::murmur_hash(name.c_str());
		for ( size_t i = 0; i < _entries.size(); ++i ) {
			if ( _entries[i].hash == hash ) {
				ret = _entries[i].value;
				return true;
			}
		}
		return false;
	}
	Vector2f getVector2f(const std::string& name,const Vector2f& defaultValue = Vector2f(1.0f,1.0f)) {
		if ( hasProperty(name)) {
			float x = getFloat(0,name);
			float y = getFloat(1,name);
			return Vector2f(x,y);
		}
		return defaultValue;
	}
	void getVector2f(const std::string& name,Vector2f* ret) {
		if ( hasProperty(name)) {
			ret->x = getFloat(0,name);
			ret->y = getFloat(1,name);
		}
	}
	void getIdString(const std::string& name,IdString* ret) {
		if ( hasProperty(name)) {
			std::string s;
			getProperty(name,s);
			*ret = string::murmur_hash(s.c_str());
		}
	}
	Vector3f getVector3f(const std::string& name) {
		float x = getFloat(0,name);
		float y = getFloat(1,name);
		float z = getFloat(2,name);
		return Vector3f(x,y,z);
	}
	Vector3f getVector3f(const std::string& name,const Vector3f& defaultValue) {
		if (hasProperty(name)) {
			float x = getFloat(0, name);
			float y = getFloat(1, name);
			float z = getFloat(2, name);
			return Vector3f(x, y, z);
		}
		return defaultValue;
	}
	/*
	ds::Color getColor(const std::string& name,const ds::Color& defaultValue = ds::Color::WHITE) {
		if ( hasProperty(name)) {
			assert(getElementCount(name) == 4);
			int r = getInt(0,name);
			int g = getInt(1,name);
			int b = getInt(2,name);
			int a = getInt(3,name);
			return ds::Color(r,g,b,a);
		}
		return defaultValue;
	}

	void getColor(const std::string& name,ds::Color* color) {
		if ( hasProperty(name)) {
			assert(getElementCount(name) == 4);
			int r = getInt(0,name);
			int g = getInt(1,name);
			int b = getInt(2,name);
			int a = getInt(3,name);
			color->r = static_cast<float>(r) / 255.0f;
			color->g = static_cast<float>(g) / 255.0f;
			color->b = static_cast<float>(b) / 255.0f;
			color->a = static_cast<float>(a) / 255.0f;
		}
	}
	*/
	Rect getRect(const std::string& name) {
		float top = static_cast<float>(getInt(0,name));
		float left = static_cast<float>(getInt(1,name));
		float width = static_cast<float>(getInt(2,name));
		float height = static_cast<float>(getInt(3,name));
		return Rect(top,left,width,height);
	}
	void getRect(const std::string& name,Rect* rect) {
		if ( hasProperty(name)) {
			rect->top = static_cast<float>(getInt(0,name));
			rect->left = static_cast<float>(getInt(1,name));
			rect->right = rect->left + static_cast<float>(getInt(2,name));
			rect->bottom = rect->top + static_cast<float>(getInt(3,name));
		}		
	}
	
	bool getBool(const std::string& name,bool defaultValue = false) {
		if ( hasProperty(name)) {
			std::string s;
			getProperty(name,s);
			if ( s == "true") {
				return true;
			}
			return false;
		}
		else {
			return defaultValue;
		}		
	}

	void getBool(const std::string& name,bool* ret) {
		if ( hasProperty(name)) {
			std::string s;
			getProperty(name,s);
			if ( s == "true") {
				*ret = true;
			}
			*ret = false;
		}		
	}
	
	template<class T> T read(const std::string& name,const T& defaultValue) {
		if ( m_Properties.find(name) == m_Properties.end() ) {
			return defaultValue;
		}
		std::string s = m_Properties[name];
		T t;
		std::istringstream ist(s);
		ist >> t;
		return t;
	}	
	
	bool hasProperty(const std::string& name) const {
		IdString hash = string::murmur_hash(name.c_str());
		for ( size_t i = 0; i < _entries.size(); ++i ) {
			if ( _entries[i].hash == hash ) {
				return true;
			}
		}
		return false;
	}
	
	void addCategory(Category* cat) {
		_children.push_back(cat);
	}
	
	std::vector<Category*>& getChildren() {
		return _children;
	}

	Category* getChild(const std::string& name) {
		IdString hash = string::murmur_hash(name.c_str());
		for ( size_t i = 0; i < _children.size(); ++i ) {
			if ( _children[i]->getName() == hash ) {
				return _children[i];
			}
		}
		return 0;
	}
	
	void getFloat(const std::string& name,float* ret) {
		if ( hasProperty(name)) {
			*ret = getFloat(0,name);
		}
	}	
	float getFloat(const std::string& name,float defaultValue = 0.0f) {
		if ( hasProperty(name)) {
			return getFloat(0,name);
		}
		return defaultValue;
	}
	
	uint32 getUInt32(const std::string& name,uint32 defaultValue) {
		if ( hasProperty(name)) {
			return static_cast<uint32>(getInt(0,name));
		}
		return defaultValue;
	}
	float getFloat(int index,const std::string& name) {
		float v = 0.0f;
		if ( hasProperty(name)) {
			std::string s;
			getProperty(name,s);
			std::vector<std::string> values;
			string::split(s,',',values);			
			std::istringstream ist(values[index]);
			ist >> v;
		}
		return v;
	}
	void getInt(const std::string& name,int* value) {
		if ( hasProperty(name)) {
			*value = getInt(0,name);
		}
	}
	int getInt(const std::string& name,int defaultValue = 0) {
		if ( hasProperty(name)) {
			return getInt(0,name);
		}
		return defaultValue;
	}
	void getInt(const std::string& name,uint32* value) {
		if ( hasProperty(name)) {
			*value = static_cast<uint32>(getInt(0,name));
		}
	}
	int getInt(int index,const std::string& name) {
		std::string s;
		getProperty(name,s);
		std::vector<std::string> values;
		string::split(s,',',values);
		int v;
		std::istringstream ist(values[index]);
		ist >> v;
		return v;
	}
	int getElementCount(const std::string& name) {
		std::string s;
		getProperty(name,s);
		std::vector<std::string> values;
		string::split(s,',',values);
		return values.size();
	}
private:
	Entries _entries;
	IdString _name;
	std::vector<Category*> _children;
};

// -------------------------------------------------------
// JSON Reader
// -------------------------------------------------------
class JSONReader {

public:
	JSONReader();
	virtual ~JSONReader();
	bool parse(const char* fileName);   
	std::vector<Category*>& getCategories() {
		return m_Categories;
	}
	Category* getCategory(const std::string& name);
	const uint32 numCategories() const {
		return m_Categories.size();
	}
	Category* getCategory(uint32 index) {
		assert(index < m_Categories.size());
		return m_Categories[index];
	}
	void clear();
private:
	std::vector<Category*> m_Categories;
};