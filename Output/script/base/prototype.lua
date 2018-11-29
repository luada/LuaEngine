--[[Writen by Luada

提供原型设计模式的支持

Exported API:
	Prototype(class, propertyObject, TemplateClass)
	
	Prototype:reader(name, default)
	Prototype:writer(name, default)
	Prototype:accessor(name, default)
	
Example:
	------------------------------------------------------
	function observer(obj, name, old, value)
		print(name, old, value)
	end

	------------------------------------------------------
	Template = class(Singleton)

	function Template:__init(object)
	end

	function Template:getName()
		return "TestName"
	end
	------------------------------------------------------

	TestClass = class()

	local prop = Property(TestClass, observer)
	local proto = Prototype(TestClass, prop, Template)

	proto:accessor("id", 0)
	proto:reader("name")


	function TestClass:__init(id)
		local this = prop[self]
		this.id = id
	end


	test = TestClass(234)
	test:setId(888)
	print(test:getName())
	------------------------------------------------------
--]]

require "base.base"

--------------------------------------------------------------------------------
local ACCESSOR = 1
local WRITER = 2
local READER = 3
--------------------------------------------------------------------------------
local classMT = {}
--------------------------------------------------------------------------------
local function prototypeAccessor(prototype, class, name, default, mode)
	--[[
	（1）通过Property方式设置class属性name的setter
	（2）当class没有属性name的getter时，属性name的值会从原型的getter函数或__getValue函数获取
	@param	prototype：class的原型
	@type	prototype：Prototype产生的实例
	@param	class：待设定的类
	@type	class：class
	@param	name：待设定的class的属性名
	@type	name：string
	@param	default：默认值
	@type	default：any
	@param	mode：访问模式
	@type	mode：ACCESSOR， WRITER， READ之一
	--]]
	---assert(class, "error Class type.")
	---assert(name, "error Name attribute.")
	local _name = string.title(name)

	local prop = rawget(prototype, "__prop")
	local method = rawget(prototype, "__method")

	if mode <= WRITER then
		prop:writer(name, default)
		mode = mode + 2
	end

	if mode <= READER then
		class["get".._name] = function(self)
			local this = prop[self]
			local ret = this[name]

			if not ret then
				local object = method(self)
				if object then
					if type(object["get".._name]) == "function" then
						ret = object["get".._name](object, self)
					else
						ret = object:__getValue(name, self, default)
					end
				end
			end

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
local function prototypeNew(_, class, prop, method)
	--[[
	Prototype的创建函数
	@param	class：待设定的类
	@type	class：class
	@param	prop：class的Property
	@type	prop：Property产生的实例
	@param	method：提供class不同通过getter方式获取的属性数据源
	@type	method：object
	--]]
	assert(class, "error Class type.")

	local prototype = define(classMT, {
		__prop = prop,
		__method = method,
		reader = function(self, name, default)
			prototypeAccessor(self, class, name, default, READER)
		end,
		writer = function(self, name, default)
			prototypeAccessor(self, class, name, default, WRITER)
		end,
		accessor = function(self, name, default)
			prototypeAccessor(self, class, name, default, ACCESSOR)
		end
	})

	return prototype
end
--------------------------------------------------------------------------------
Prototype = define { __call = prototypeNew }