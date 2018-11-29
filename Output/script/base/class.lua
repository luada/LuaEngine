--[[Writen by Luada

提供类的继承机制，支持多重继承。

Exported API:
	class(super, ...)
	release(object)
	classof(object)
	className(object)
	islass(class)
	delegate(object, class)
	instanceof(object, class)
	superclass(class)
	subclassof(class, super)
	subclassofInterface(class, interface)
	getInterfaces(class)
	checkImplemented(class)
	isClassInstance(object)

Example:
	local Object = class()

	function Object:__init(id)
		print("object:__init", id)
		self.id = id
	end

	function Object:__release()
		print("object:__release")
	end

	function Object:getId()
		return self.id
	end

	obj = Object(123)
	obj:release(obj)
	obj = nil
--]]

require "base.try"
require "base.base"
require "base.interface"
require "base.singleton"
require "base.serialize"
require "base.property"
require "base.prototype"

--------------------------------------------------------------------------------
local advance = true
local classMap = define { __mode = "k" }
--------------------------------------------------------------------------------
local function rawnew(class, object, ...)
	--[[
	调用由类class产生的对象object的初始化函数__init
	@param	class: 类
	@type	class：class
	@param	object：由class产生的对象
	@type	object：object
	@param	arg：初始化函数__init的参数列表
	@type	arg：参数列表
	@param	return：初始化后的object
	@type	return：object
	--]]
	
	--[[--------------------------------------------------------------------------
	--Begin方案1
		--调用顺序：基类-》子类
	---（1）调用基类的初始化函数
	for i = 1, #class.__supers do
		local super = class.__supers[i]
		if isclass(super) then
			rawnew(super, object, ...)
		end
	end
	---(2) 调用子类的初始化函数
	if type(class.__init) == "function" then
		class.__init(object, ...)
	end
	return object
	--End方案1
	--]]--------------------------------------------------------------------------
	
	---Begin方案2
		---仅调用子类最新版本的初始化函数__init，由用户自己决定基类的初始化函数的调用顺序和参数（灵活）
	if type(class.__init) == "function" then
		class.__init(object, ...)
	end
	return object
	---End方案2
	
end
--------------------------------------------------------------------------------
local function new(class, ...)
	--[[
	由class产生的对象时调用的函数
	@param	class: 类
	@type	class：class
	@param	arg：初始化函数__init的参数列表
	@type	arg：参数列表
	@param	return：由class产生的新对象
	@type	return：object
	--]]
	---(0) 加速单件实例的检索
	local inst = rawget(class, "__instance")
	if inst ~= nil then
		return inst	
	end
	
	---（1）检查class所有的接口是否都被实现了
	if not class.__implemented then
		advance = false
		local ret, method = checkImplemented(class)
		advance = true
		if not ret then
				error(string.format("The %q is not implemented!", method))
			end
		class.__implemented = true
	end
	
	---（2）检查class是否派生于单件Singleton类。如果是则内存中只有一份产生的对象；否则正常产生新对象
	if inheritfrom(class, Singleton) then
		local object = define(class.__vtbl)
		inst = rawnew(class, object, ...)
		rawset(class, "__instance", inst)
		return inst
	else
		local object = define(class.__vtbl)
		return rawnew(class, object, ...)
	end
end
--------------------------------------------------------------------------------
local function rawrelease(class, object)
	--[[
	调用由类class产生的对象object的释放函数__release；调用顺序：子类-》基类
	@param	class: 类
	@type	class：class
	@param	object：由class产生的对象
	@type	object：object
	--]]
	---（1）调用子类的释放函数
	if type(class.__release) == "function" then
		class.__release(object)
	end
	---（2）调用基类的释放函数
	for i = 1, #class.__supers do
		local super = class.__supers[i]
		if isclass(super) then
			rawrelease(super, object)
		end
	end
end
--------------------------------------------------------------------------------
function release(object)
	--[[
	释放object；调用此函数后，还得将所有引用object的变量置为nil
	@param	object：
	@type	object：object
	--]]
	if isInterfaceInstance(object) then
		object:release()
		return
	end
	
	local class = classof(object)
	if class then
		rawrelease(class, object)
		if inheritfrom(class, Singleton) then
			rawset(class, "__instance", nil)
		end
	end
end
--------------------------------------------------------------------------------
local function supersToStr(supers, members)
	--[[
	将class字符串化的辅助函数
	@param	supers：由基类组成的表
	@type	supers：table
	@param	members：标志class的__vtbl键是否被访问过
	@type	members：table
	@param	return: class字符串化
	@type	return：string
	--]]
	local ret = StringBuffer("")
	local separator = ""
	for i = 1, #supers do
		local super = supers[i]
		local vtbl = super.__vtbl
		for f,v in pairs(vtbl) do
			if not members[f] and not string.startsWith(f, "_") then
				if type(v) == "string" then
					ret = ret .. separator .. f .. " = " .. string.format("%q", v)
				else
					ret = ret .. separator .. f .. " = " .. tostring(v)
				end
				separator = ", "
			end
			members[f] = true
		end
		local superRet = supersToStr(super.__supers, members)
		ret = ret .. separator
		ret = ret .. superRet
	end
	return ret
end
--------------------------------------------------------------------------------
local classMT = {
	__call = new,
	__index = function(class, field)
		local value = class.__vtbl[field]
		if advance and value and not string.startsWith(field, "_") then
			rawset(class, field, value)
		end
		return value
	end,
	__newindex = function(class, field, value)
		class.__vtbl[field] = value
	end,
	__tostring = function(class)
		local members = {}
		local supers = {class}
		local ret = StringBuffer("<<class>> = {") .. supersToStr(supers, members) .. "}"
		return tostring(ret)
	end
}
--------------------------------------------------------------------------------
local function objectToStr(object)
	--[[
	将object字符串化
	@param	object: 
	@type	object: object
	@param	return: object字符串化
	@type	return：string
	--]]
	if type(object.tostring) == "function" then
		return object:tostring()
	end

	local separator = ""
	local ret = StringBuffer("<<object>> = {")
	for f,v in pairs(object) do
		if not string.startsWith(f, "_") then
			if type(v) == "string" then
				ret = ret .. separator .. f .. " = " .. string.format("%q", v)
			else
				ret = ret .. separator .. f .. " = " .. tostring(v)
			end
			separator = ", "
		end
	end
	ret = ret .. "}"
	return tostring(ret)
end
--------------------------------------------------------------------------------
local function searchSupersMethod(k, supers)
	--[[
	在基类supers中查找键为k的值
	@param	k: 键值
	@type	k: string or number
	@param	supers: 由基类组成的表
	@type	supers: table
	--]]
	for i=1, #supers do
		local v = supers[i][k]    -- try 'i'-th superclass
		if v then
			return v
		end
	end
end
--------------------------------------------------------------------------------
function delegate(object, class)
	--[[
	向Object对象挂接一个Class的委托；实现的功能：object没有找到方法时，可以在Class上找
	@param	object：被挂接的实例
	@type	object：object of Class
	@param	class：类
	@type	class：Class
	--]]
    local mt = getmetatable(object)
    setmetatable(object, {
        __index = function(t, k)
			local v = mt[k]
			return v or class[k]
        end
    })
end
--------------------------------------------------------------------------------
function class(...)
	--[[
	类的定义函数 SubClass = class(Superclass / Interfaces)
	@param	arg: 要被继承的基类(接口)
	@type	arg: 由class,interface组成的参数列表
	@param	return: 继承关系定义后的子类
	@type	return: class
	--]]
	---(1)分配superclass和interface
	local interfaces = {}
	local supers = {}
	for i = 1, select("#", ...) do
		local curArg = select(i, ...)
		if isclass(curArg) then
			supers[#supers + 1] = curArg
		elseif isInterface(curArg) then
			interfaces[#interfaces + 1] = curArg
		else
			error(string.format("Wrong type @ param:%s !Param must be an interface or a class!", i))
		end
	end

	local vtbl = {
		__tostring = objectToStr,
		release = release,
	}

	vtbl.__index = vtbl

	local class = define(classMT, {
		__supers = supers,
		__interfaces = interfaces,
		__vtbl = vtbl,
		__init = false,
		__release = false,
	})

	classMap[vtbl] = class

	if #class.__supers > 0 then
		define({
		--[[
		方案一：子类中缓存没有扩展实现的基类的方法，用于加速搜索
		---	__index = function (t, k) local v = searchSupersMethod(k, class.__supers) t[k] = v return v end
		方案二：子类中没有的方法在基类中即时查找，用于节省内存
		---	__index = function (t, k) local v = searchSupersMethod(k, class.__supers) return v end
		--]]
		--- PVE采用第一种方案
			__index = function (t, k) local v = searchSupersMethod(k, class.__supers) t[k] = v return v end
		}, vtbl)
	end

	return class
end
--------------------------------------------------------------------------------
function isclass(class)
	--[[
	判断class是否为类
	@param	class：
	@type	class: class
	@param	return: 判断结果
	@type	return：boolean（true or false）
	--]]
	local mt = getmetatable(class)
	if mt == classMT then return true end
	if classMap[mt] then return true end
	return false
end
--------------------------------------------------------------------------------
function classof(object)
	--[[
	获取对象object的类
	@param	object：
	@type	object：object
	@param	return：产生object的类
	@type	return：class
	--]]
	local mt = getmetatable(object)
	return classMap[mt]
end
--------------------------------------------------------------------------------
function className(object)
	--[[
	在全局区搜索实例object的类名字
	@param	object：
	@type	object：object
	@param	return：查实成功时，返回类名字；否则返回""
	@type	return：string
	--]]
	return table.getKeyName(_G, object.__interface or classof(object))
end
--------------------------------------------------------------------------------
function isClassInstance(object)
	--[[
	判断object是否为类产生的实例
	@param	return：
	@type	return：boolean（true or false）
	--]]
	return classof(object) ~= nil
end
--------------------------------------------------------------------------------
function getInterfaces(class)
	--[[
	获取被class继承的接口
	@param	class:
	@type	class:
	@param	return: 被class继承的接口
	@type	return：interface
	--]]
	return rawget(class, "__interfaces")
end
--------------------------------------------------------------------------------
function superclass(class)
	--[[
	获取被class继承的基类
	@param	class:
	@type	class:
	@param	return: 被class继承的基类
	@type	return：class
	--]]
	return rawget(class, "__supers")
end
--------------------------------------------------------------------------------
function subclassof(class, super)
	--[[
	判断class是否继承super
	@param	class：子类
	@type	class：class
	@param	super：基类
	@type	super：class
	@param	return：class继承super或者和super为同一类时，返回true；否则为false
	@type	return：boolean(true or false)
	--]]
	if class == super then
		return true
	end

	for _, s in pairs(superclass(class)) do
		if subclassof(s, super) then
			return true
		end
	end

	return false
end
--------------------------------------------------------------------------------
function subclassofInterface(class, interface)
	--[[
	检查class是否继承接口interface
	@param	class：子类
	@type	class：class
	@param	interface：要被测试的接口
	@type	interface：interface
	@param	return：class继承interfaces时返回true；否则为false
	@type	return: boolean(true or false)
	--]]
	local interfaces = getInterfaces(class)
	for i = 1, #interfaces do
		if subinterfaceof(interfaces[i], interface) then
			return true
		end
	end

	for _, s in pairs(superclass(class)) do
		if subclassofInterface(s, interface) then
			return true
		end
	end

	return false
end
--------------------------------------------------------------------------------
function checkImplemented(class, super)
	--[[
	检查类class所有的接口是否都被实现了
	@param	class:
	@type	class: class
	@param	super: 被class继承的基类；首次调用，参数super必须为nil
	@type	super：class
	@param	return：class所有的接口都被实现了，则返回（true，nil）否则返回（false， 未实现的接口名字）
	@type	return：（boolean，string or nil）
	--]]
	local ret, method = implemented(class, unpack(getInterfaces(super and super or class)))
	if not ret then
		return ret, method
	end
	
	for _, s in pairs(superclass(super and super or class)) do
		ret, method = checkImplemented(class, s)
		if not ret then
			return ret, method
		end
	end
	
	return true
end
--------------------------------------------------------------------------------
function instanceof(object, class)
	--[[
	判断对象object是否为class产生出来的实例
	@param	object：被测试对象，或者派生类
	@type	object：object
	@param	class：
	@type	class：class
	@param	return：测试结果
	@type	return：boolean（true or false)
	--]]
	if object then
		if isInterfaceInstance(object) then
			return subinterfaceof(object.__interface, class)
		elseif isClassInstance(object) then
			if isclass(class) then
				return subclassof(classof(object), class)
			elseif isInterface(class) then
				return subclassofInterface(classof(object), class)
			end
		end
	end
	return false
end
--------------------------------------------------------------------------------
function inheritfrom(sub, super)
	--[[
	判断sub是否继承于super
	@param	sub: 子类
	@type	sub: class or interface
	@param	super: 基类
	@type	super: class or interface
	@param	return:测试结果
	@type	return:boolean（true or false)
	--]]
	if sub == nil or super == nil then
		return false
	end
	
	if isclass(sub) then
		if isclass(super) then
			return subclassof(sub, super)
		elseif isInterface(super) then
			return subclassofInterface(sub, super)
		end
	elseif isInterface(sub) then
		if isInterface(super) then
			return subinterfaceof(sub, super)
		end
	end
	
	return false
end
--------------------------------------------------------------------------------
local _toString = toString
function toString(value)
	--[[
	将value字符串化
	@param	value：
	@type	value：any
	@param	return: value字符串化
	@type	return：string
	--]]
	local sb = StringBuffer()
	if isclass(value) or isInterface(value) or classof(value) then
		sb:append(tostring(value))
	else
		sb:append(_toString(value)) ---Proper Tail Call
	end
	return tostring(sb)
end

--------------------------------------------------------------------------------

require "base.exception"
require "base.logger"

--------------------------------------------------------------------------------
