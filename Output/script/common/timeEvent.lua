--[[Created by Luada
时间事件
--]]

require "common.timerMgr"

TimeEvent = class()

local prop = Property(TimeEvent)
prop:accessor("delay")
prop:accessor("timeout")

function TimeEvent:__init(delay)
	--[[
	delay几秒后响应超时处理;delay为负数时，表示以-delay为一周期，不断响应超时处理
	@param	delay：
	@type	delay：float，单位：秒
	--]]
	local this = prop[self]
	this.delay = delay
	self:reset()
end

function TimeEvent:__release()
end

function TimeEvent:needReset()
	--[[
	判断是否需要重置超时处理
	--]]
	local this = prop[self]
	return this.delay < 0
end

function TimeEvent:reset()
	--[[
	重置超时处理
	--]]
	local this = prop[self]
	local delay = this.delay
	this.timeout = TimerMgr():getCurTime() + (delay < 0 and -delay or delay)
end

function TimeEvent:onAdd()
	--[[
	加入TimerMgr时的回调函数
	--]]
end

function TimeEvent:onRemove()
	--[[
	从TimerMgr挪出时的回调函数
	--]]
end

function TimeEvent:onTimeout()
	--[[
	超时处理；具体效果放在子类上来实现
	--]]
end