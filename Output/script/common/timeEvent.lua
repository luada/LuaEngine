--[[Created by Luada
ʱ���¼�
--]]

require "common.timerMgr"

TimeEvent = class()

local prop = Property(TimeEvent)
prop:accessor("delay")
prop:accessor("timeout")

function TimeEvent:__init(delay)
	--[[
	delay�������Ӧ��ʱ����;delayΪ����ʱ����ʾ��-delayΪһ���ڣ�������Ӧ��ʱ����
	@param	delay��
	@type	delay��float����λ����
	--]]
	local this = prop[self]
	this.delay = delay
	self:reset()
end

function TimeEvent:__release()
end

function TimeEvent:needReset()
	--[[
	�ж��Ƿ���Ҫ���ó�ʱ����
	--]]
	local this = prop[self]
	return this.delay < 0
end

function TimeEvent:reset()
	--[[
	���ó�ʱ����
	--]]
	local this = prop[self]
	local delay = this.delay
	this.timeout = TimerMgr():getCurTime() + (delay < 0 and -delay or delay)
end

function TimeEvent:onAdd()
	--[[
	����TimerMgrʱ�Ļص�����
	--]]
end

function TimeEvent:onRemove()
	--[[
	��TimerMgrŲ��ʱ�Ļص�����
	--]]
end

function TimeEvent:onTimeout()
	--[[
	��ʱ��������Ч��������������ʵ��
	--]]
end