--[[Created by Luada
ʱ���¼�����ģ��
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
	�������
	--]]
	for _, v in ipairs(self._events) do
		v:release()
	end
	self._events = {}
end

function TimerMgr:getCurTime()
	--[[
	��ȡ��Ϸ�߼����е�ʱ�䣬��λ���룩
	@param	return��
	@type	return��float
	--]]
	return self._curTime
end

function TimerMgr:addEvent(event)
	--[[
	���һ����ʱ�����¼�
	@param	event��
	@type	event��object of TimeEvent
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
	ɾ��һ����ʱ�����¼�
	@param	event:
	@type	event: object of TimeEvent
	--]]
	table.rmArrayValue(self._events, event)
	event:onRemove()
end

function TimerMgr:tick(elapse)
	--[[
	ʱ���¼����������߼����㴦��
	@param	elapse�����δ�����һ�δ����ʱ����
	@type	elapse��float����λ����
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