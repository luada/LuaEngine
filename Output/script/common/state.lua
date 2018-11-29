--[[Writen by Luada

FSM(Finite State Machine)的状态接口

--]]

require "base.class"

IState = interface(nil, 
			"getName",		---getName(); 获取状态的名字；返回值类型string
			"onEnter",		---onEnter(); 进入此状态时的回调;没有返回值
			"onLeave",		---onLeave(); 退出此状态时的回调;没有返回值
			"onExecute"		---onExecute();状态执行的回调函数;没有返回值
			)

--------------------------------------------------------------------------------

---空状态，单件模式；不处理任何事情
DummyState = class(IState, Singleton)

function DummyState:__init()
end

function DummyState:__release()
end

function DummyState:getName()
	--[[
	获取状态的名字
	@param	return:
	@type	return: string
	--]]
	return "DummyState"
end

function DummyState:onEnter()
	--[[
	进入此状态时的回调
	--]]
end

function DummyState:onLeave()
	--[[
	退出此状态时的回调
	--]]
end

function DummyState:onExecute()
	--[[
	状态执行的回调函数;
	--]]
end