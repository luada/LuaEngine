--[[Writen by Luada

有限状态机（Finite State Machine,简称FSM）模块

--]]

require "common.state"


FSM = class()

function FSM:__init()
	self._state = DummyState()
end

function FSM:__release()
	self._state = nil
end

function FSM:getStateName()
	--[[
	获取当前状态的名字
	@param	return：
	@type	return：string
	--]]
end

function FSM:execute()
	--[[
	通过有限状态机来执行当前状态
	--]]
	self._state:onExecute()
end

function FSM:changeState(state)
	--[[
	状态切换处理
	@param	state: 新状态
	@type	state: object of IState or nil
	--]]
	self._state:onLeave()
	self._state = state and state or DummyState()
	self._state:onEnter()
end