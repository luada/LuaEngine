--[[Writen by Luada

�ṩԭ�����ģʽ��֧��

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
	��1��ͨ��Property��ʽ����class����name��setter
	��2����classû������name��getterʱ������name��ֵ���ԭ�͵�getter������__getValue������ȡ
	@param	prototype��class��ԭ��
	@type	prototype��Prototype������ʵ��
	@param	class�����趨����
	@type	class��class
	@param	name�����趨��class��������
	@type	name��string
	@param	default��Ĭ��ֵ
	@type	default��any
	@param	mode������ģʽ
	@type	mode��ACCESSOR�� WRITER�� READ֮һ
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
	Prototype�Ĵ�������
	@param	class�����趨����
	@type	class��class
	@param	prop��class��Property
	@type	prop��Property������ʵ��
	@param	method���ṩclass��ͬͨ��getter��ʽ��ȡ����������Դ
	@type	method��object
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