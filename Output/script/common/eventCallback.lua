--[[Writen by Luada

事件监听回调类

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
	向事件管理器注册本类的事件
	--]]
	local EC = ECenter()
	local this = prop[self]
	for eventKey, _ in pairs(this.eventCallbacks_) do
		EC:registerEvent(eventKey, self)
	end
end

function EventCallback:deregisterTriggers_()
	--[[
	向事件管理器注销本类的事件
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
	消息事件回调处理
	@param	eventKey: 事件名字
	@type	eventKey: eventType
	@param	arg: 触发事件回调函数的参数列表
	@type	arg: 根据实际的触发事件回调函数而定
	@param	return：返回值为True时，本组（eventKey）事件往下触发；否则终止本组事件的触发
	@type	return: boolean
	--]]
	local this = prop[self]
	this.curEventKey = eventKey
	return this.eventCallbacks_[eventKey](self, ...)
end