--[[Writen by Luada

����״̬����Finite State Machine,���FSM��ģ��

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
	��ȡ��ǰ״̬������
	@param	return��
	@type	return��string
	--]]
end

function FSM:execute()
	--[[
	ͨ������״̬����ִ�е�ǰ״̬
	--]]
	self._state:onExecute()
end

function FSM:changeState(state)
	--[[
	״̬�л�����
	@param	state: ��״̬
	@type	state: object of IState or nil
	--]]
	self._state:onLeave()
	self._state = state and state or DummyState()
	self._state:onEnter()
end