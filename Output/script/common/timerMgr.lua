--[[Created by Luada
时间事件管理模块
--]]

TimerMgr = class(Singleton, ITick)

function TimerMgr:__init()
	self._curTime = 0
	self._events = {}
	Ticker():addTick(self)
end

function TimerMgr:__release()
	self._events = nil
	Ticker():removeTick(self)
end

function TimerMgr:clear()
	--[[
	清理操作
	--]]
	for _, v in ipairs(self._events) do
		v:release()
	end
	self._events = {}
end

function TimerMgr:getCurTime()
	--[[
	获取游戏逻辑运行的时间，单位（秒）
	@param	return：
	@type	return：float
	--]]
	return self._curTime
end

function TimerMgr:addEvent(event)
	--[[
	添加一个超时处理事件
	@param	event：
	@type	event：object of TimeEvent
	--]]
	for _, v in ipairs(self._events) do
		if v == event then
			return
		end
	end
	table.insert(self._events, event)
	event:onAdd()
end

function TimerMgr:removeEvent(event)
	--[[
	删除一个超时处理事件
	@param	event:
	@type	event: object of TimeEvent
	--]]
	table.rmArrayValue(self._events, event)
	event:onRemove()
end

function TimerMgr:tick(elapse)
	--[[
	时间事件管理器的逻辑运算处理
	@param	elapse：本次处理到上一次处理的时间间隔
	@type	elapse：float，单位：秒
	--]]
	local curTime = self._curTime + elapse
	self._curTime = curTime
	local rmEvts = {}
	local timeOutEvts = {}
	for _, v in ipairs(self._events) do
		if v:getTimeout() <= curTime then
			table.insert(timeOutEvts, v)
			if v:needReset() then
				v:reset()
			else
				table.insert(rmEvts, v)
			end
		end
	end
	
	for _, v in ipairs(rmEvts) do
		self:removeEvent(v)
	end
	
	for _, v in ipairs(timeOutEvts) do
		v:onTimeout()
	end
end