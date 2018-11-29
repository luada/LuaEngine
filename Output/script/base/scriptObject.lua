--[[Writen by Luada
 
底层对象和脚本层对象的粘合类

--]]

require "base.class"

--------------------------------------------------------------------------------

IScriptObject = interface(nil, 
			"__onEngineObjLost", 
			"__onEngineObjAttach", 
			"__onLoaded",
			"New",
			"getScriptClass",
			"attachEngineObj")

--------------------------------------------------------------------------------

InvalidScriptID = -2

--------------------------------------------------------------------------------

ScriptObject = class(IScriptObject)

function ScriptObject:__init()
	--[[
	初始化对象时的回调函数
	--]]
end

function ScriptObject:__onLoaded()
	--[[
	对象被引擎加载数据后的回调函数
	--]]
end

function ScriptObject:__release()
	--[[
	释放对象时的回调函数
	--]]
end

function ScriptObject:__onEngineObjLost()
	--[[
	挂接的引擎对象丢失时，回调该函数.
	--]]
	if self.engineObj_ ~= nil then
		self.engineObj_:SetScriptObjID(InvalidScriptID)
		self.engineObj_ = nil
	end
end

function ScriptObject:__onEngineObjAttach(engineObj)
	--[[
	调用ScriptObject:attachEngineObj或CScriptObject:CreateScriptObj时的回调函数
	@param	engineObj:
	@type	engineObj: CScriptObject
	--]]
	self.engineObj_ = engineObj
end

function ScriptObject.New()
	--[[
	static method
	创建对象
	--]]
	local obj = ScriptObject()
	return obj
end

function ScriptObject:getScriptClass()
	--[[
	获取类的名称，该方法需要被子类重载
	--]]
	return "ScriptObject"
end

function ScriptObject:attachEngineObj(engineObj)
	--[[
	将脚本对象挂接到引擎对象
	@param	engineObj: 引擎对象
	@type	engineObj: CScriptObject
	--]]
	if engineObj:IsScriptObjValid() then
		local scriptObjID = engineObj:GetScriptObjID()
		local scriptObj = GetRegistryObj(scriptObjID)
		scriptObj:__onEngineObjLost()
	end
	engineObj:SetScriptObjID(NewRegistryRef(self))
	engineObj:SetScriptClass(getScriptClass())
	self:__onEngineObjAttach(engineObj)
end

--------------------------------------------------------------------------------

function autoWrapEngine(scriptClassName, engineObjName, engineClass)
	--[[
	自动封装引擎类方法到脚本类上
	@param	scriptClassName: 脚本类名
	@type	scriptClassName: string
	@param	engineObj: 封装接口的引擎对象名
	@type	engineObj: string
	@param	engineClass: 引擎类
	@type	engineClass: engineClass
	--]]
	for k, v in pairs(engineClass) do
		if type(v) == "function" and k ~= "__index" and k ~= "__newindex" and 
		   k ~= "new" and k ~= "new_local" and
		   k ~= ".call" and k ~= "__call" and 
		   k ~= ".collector" and k ~= "__gc" then
			local strFunc = string.format("function %s:%s(...) return self.%s:%s(...) end", scriptClassName, k, engineObjName, k)
			dobuffer(strFunc)
		end
	end
end


function autoImpScriptClass(driveClassName, baseClassName, ...)
	--[[
	自动完成继承baseClassName的driveClassName的声明，实现“driveClassName.New”和
	“driveClassName:getScriptClass”方法,并自动封装引擎对象(可变参数列表arg中)
	@param	driveClassName: 脚本子类名
	@type	driveClassName: string
	@param	baseClassName: 被继承的基类名
	@type	baseClassName: string
	@param	arg: 可变参数表，存放引擎类
	@type	arg: table of engineClass
	--]]
	local strDimClass = string.format("%s = class(%s)", driveClassName, baseClassName)
	local strFuncNew = string.format("function %s.New() return %s() end", driveClassName, driveClassName)
	local strFuncGetClass = string.format("function %s:getScriptClass() return \"%s\" end", driveClassName, driveClassName)
	local strFuncInit = string.format("function %s:__init() return %s.__init(self) end", driveClassName, baseClassName)
	dobuffer(strDimClass)
	dobuffer(strFuncNew)
	dobuffer(strFuncGetClass)
	dobuffer(strFuncInit)
	
	for i = 1, select("#", ...) do
		local v = select(i, ...)
		autoWrapEngine(driveClassName, "engineObj_", v)
	end
end

--------------------------------------------------------------------------------