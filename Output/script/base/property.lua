--[[Writen by Luada

用于属性方法声明

Exported API:
	Property(class, observer)

	Property:reader(propertyName, propertyDefaultValue)
	Property:writer(propertyName, propertyDefaultValue)
	Property:accessor(propertyName, propertyDefaultValue)

Example:
	function observer(obj, name, old, value)
		print(name, old, value)
	end

	TestClass = class()

	local prop = Property(TestClass, observer)
	prop:accessor("id")


	function TestClass:__init(id)
		local this = prop[self]
		this.id = id
	end


	test = TestClass(234)
	test:setId(888)
--]]

require "base.base"

--------------------------------------------------------------------------------
local ACCESSOR = 1
local WRITER = 2
local READER = 3
--------------------------------------------------------------------------------
local observerMap = define { __mode = "k" }
--------------------------------------------------------------------------------
local classMT = {
	__call = function(prop, obj, name, value)
		obj[name] = value
	end,
	__index = function(prop, obj)
		return obj
	end
}
--------------------------------------------------------------------------------
local function propAccessor(prop, class, name, default, mode)
	--[[
	设置属性的访问函数；设置class属性的setter和getter方法，setter方法会告知observer
	@param	prop： Property产生的实例
	@type	prop：prop Object
	@param	class：通过Property方式设置访问属性的类
	@type	class：class
	@param	name：属性名字
	@type	name：string
	@param	default：属性默认值
	@type	default：any
	@param	mode：访问权限
	@type	mode：ACCESSOR， WRITER， READ之一
	--]]
	---assert(class, "error Class type.")
	---assert(name, "error Name attribute.")
	local _name = string.title(name)

	if mode <= WRITER then
		class["set".._name] = function(self, value)
			local old = self[name]
			self[name] = value
			local observer = rawget(observerMap[classof(self)] or prop, "__observer")
			if observer then
				observer(self, name, old, value)
			end
		end
		mode = mode + 2
	end

	if mode <= READER then
		class["get".._name] = function(self)
			local ret = self[name]
			if ret ~= nil then
				return ret
			else
				return default
			end
		end
		if type(default) == "boolean" then
			class["is".._name] = class["get".._name]
		end
	end
end
--------------------------------------------------------------------------------
local function propNew(_, class, observer)
	--[[
	产生Property实例的函数
	@param	class：通过Property方式设置访问属性的类
	@type	class：class
	@param	observer：class通过setter方式改变属性时，会通知observer；使用方法见本文件开始的Example
	@type	observer：function or functor
	--]]
	assert(class, "error Class type.")

	local prop = define(classMT, {
		__class = class,
		__observer = observer,
		reader = function(self, name, default)
			propAccessor(self, class, name, default, READER)
		end,
		writer = function(self, name, default)
			propAccessor(self, class, name, default, WRITER)
		end,
		accessor = function(self, name, default)
			propAccessor(self, class, name, default, ACCESSOR)
		end
	})

	if observer then
		observerMap[class] = prop
	end

	return prop
end

Property = define { __call = propNew }
