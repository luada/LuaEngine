--[[Writen by Luada

事件触发模块

--]]

require "base.class"

--------------------------------------------------------------------------------
local Event = class()

function Event:__init()
	self._listeners = {}
end

function Event:__release()
	self._listeners = nil
end

function Event:fire(eventKey, ...)
	--[[
	触发本组事件
	@param	eventKey: 事件名字
	@type	eventKey: eventKey
	@param	arg: 监听函数的参数列表
	@type	arg: 根据实际的监听函数而定
	@param	return: 有一监听者中断了触发链时返回false；否则返回true
	@type	return: boolean(true or false)
	--]]
	for _, listener in ipairs(self._listeners) do
		if not listener:onEvent(eventKey, ...) then
			return	false
		end
	end
	return true
end

function Event:addListener(listener)
	--[[
	在本组事件中添加监听者
	@param	listener: 响应事件的监听对象
	@type	listener: object
	--]]
	table.insert(self._listeners, listener)
	
	function comparator(a, b)
		return a:getPriority() > b:getPriority()
	end
	
	table.sort(self._listeners, comparator)
end

function Event:getListener(name)
	--[[
	根据名字，获取监听者实例
	@param	name：监听者名字
	@type	name：string
	@param	return：监听者实例；没有找到时，返回nil
	@type	return：object of IEventListener or nil
	--]]
	for _, listener in ipairs(self._listeners) do
		if listener:getName() == name then
			return listener
		end
	end
	return nil
end

function Event:removeListener(listener)
	--[[
	将listener对象从本组事件中删除
	@param	listener: 响应事件的监听对象
	@type	listener: object
	--]]
	table.rmArrayValue(self._listeners, listener)
end

function Event:hasListeners()
	--[[
	判断本组事件中是否存在监听对象
	--]]
	return self._listeners ~= nil and #self._listeners > 0
end

--------------------------------------------------------------------------------
ECenter = class(Singleton)

function ECenter:__init()
	self._events = {}
end

function ECenter:__release()
	for _, event in pairs(self._events) do
		event:release()
	end
	self._events = nil
end

function ECenter:registerEvent(eventKey, listener)
	--[[
	注册一个事件
	@param  eventKey: 事件名字
	@type	eventKey: eventType
	@param	listener: 响应事件的监听对象
	@type	listener: object
	--]]
	local event = self._events[eventKey]
	if event == nil then
		event = Event()
		self._events[eventKey] = event
	end
	event:addListener(listener)
end

function ECenter:deregisterEvent(eventKey, listener)
	--[[
	注销一个事件
	@param  eventKey: 事件名字
	@type	eventKey: eventType
	@param	listener: 响应事件的监听对象
	@type	listener: object
	--]]
	local event = self._events[eventKey]
	if event ~= nil then
		event:removeListener(listener)
		if not event:hasListeners() then
			event:release()
			self._events[eventKey] = nil
		end
	end
end

function ECenter:getListener(name, eventKey)
	--[[
	根据名字和事件类型，获取监听者。当事件类型参数为nil时，会进行全局搜索
	@param	name：监听者名字
	@type	name：string
	@param	eventKey: 事件类型，可以为nil
	@type	eventKey：eventType or nil
	@param	return：返回值两个（监听者实例, table{事件类型})；没有找到时，返回nil
	@type	return：(object of IEventListener, table{eventType}) or nil
	--]]
	if eventKey ~= nil then
		local event = self._events[eventKey]
		if event ~= nil then
			local listener = event:getListener(name)
			if listener ~= nil then
				return listener, {eventKey}
			end
		end
		return nil
	end
	
	local findListener = nil
	local findEventKeys = {}
	for eventKey, event in pairs(self._events) do
		local listener = event:getListener(name)
		if listener ~= nil then
			assert(findListener == nil or listener == findListener, 
					string.format("The name(%s) of Listener is not unique!", name))
			findListener = listener
			table.insert(findEventKeys, eventKey)
		end
	end
	
	if findListener ~= nil then
		return findListener, findEventKeys
	end
	
	return nil
end

function ECenter:fireEvent(eventKey, ...)
	--[[
	触发一组事件，直到有一监听者中断了触发链
	@param	eventKey: 事件名字
	@type	eventKey: eventType
	@param	arg: 触发函数的参数列表
	@type	arg: 根据实际的触发函数而定
	@param	return: 有一监听者中断了触发链时返回false；否则返回true
	@type	return: boolean(true or false)
	--]]
	local event = self._events[eventKey]
	if event ~= nil then
		return event:fire(eventKey, ...)
	end
	return true
end

--------------------------------------------------------------------------------