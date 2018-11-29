--[[Writen by Luada

�¼������ص���

--]]

require "common.eventListener"


EventCallback = class(IEventListener)

local prop = Property(EventCallback)
prop:reader("name")
prop:reader("priority")
prop:reader("curEventKey")

function EventCallback:__init(name, priority)
	local this = prop[self]
	this.name = name
	this.priority = priority
	this.eventCallbacks_ = {}
end

function EventCallback:__release()
	local this = prop[self]
	this:deregisterTriggers_()
	this.eventCallbacks_ = nil
end

function EventCallback:registerTriggers_()
	--[[
	���¼�������ע�᱾����¼�
	--]]
	local EC = ECenter()
	local this = prop[self]
	for eventKey, _ in pairs(this.eventCallbacks_) do
		EC:registerEvent(eventKey, self)
	end
end

function EventCallback:deregisterTriggers_()
	--[[
	���¼�������ע��������¼�
	--]]
	local this = prop[self]
	local EC = ECenter()
	for eventKey, _ in pairs(this.eventCallbacks_) do
		EC:deregisterEvent(eventKey, self)
	end
	this.eventCallbacks_ = {}
end

function EventCallback:onEvent(eventKey, ...)
	--[[
	��Ϣ�¼��ص�����
	@param	eventKey: �¼�����
	@type	eventKey: eventType
	@param	arg: �����¼��ص������Ĳ����б�
	@type	arg: ����ʵ�ʵĴ����¼��ص���������
	@param	return������ֵΪTrueʱ�����飨eventKey���¼����´�����������ֹ�����¼��Ĵ���
	@type	return: boolean
	--]]
	local this = prop[self]
	this.curEventKey = eventKey
	return this.eventCallbacks_[eventKey](self, ...)
end