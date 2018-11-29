--[[Writen by Luada

FSM(Finite State Machine)��״̬�ӿ�

--]]

require "base.class"

IState = interface(nil, 
			"getName",		---getName(); ��ȡ״̬�����֣�����ֵ����string
			"onEnter",		---onEnter(); �����״̬ʱ�Ļص�;û�з���ֵ
			"onLeave",		---onLeave(); �˳���״̬ʱ�Ļص�;û�з���ֵ
			"onExecute"		---onExecute();״ִ̬�еĻص�����;û�з���ֵ
			)

--------------------------------------------------------------------------------

---��״̬������ģʽ���������κ�����
DummyState = class(IState, Singleton)

function DummyState:__init()
end

function DummyState:__release()
end

function DummyState:getName()
	--[[
	��ȡ״̬������
	@param	return:
	@type	return: string
	--]]
	return "DummyState"
end

function DummyState:onEnter()
	--[[
	�����״̬ʱ�Ļص�
	--]]
end

function DummyState:onLeave()
	--[[
	�˳���״̬ʱ�Ļص�
	--]]
end

function DummyState:onExecute()
	--[[
	״ִ̬�еĻص�����;
	--]]
end