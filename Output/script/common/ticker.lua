--[[Created by Luada
�߼����㴦��ģ��
--]]

--------------------------------------------------------------------------------

ITick = interface(nil, "tick")


--------------------------------------------------------------------------------
Ticker = class(Singleton, ITick)

function Ticker:__init(getElapseFn)
	self._getElapse = getElapseFn
	self._time = self._getElapse()
	self._ticks = {}
end

function Ticker:__release()
	self._ticks = nil
end

function Ticker:_elapse()
	--[[
	��ȡʱ����
	@param	return��
	@type	return��float����λ����
	--]]
	local curTime = self._getElapse()
	local elapse = curTime - self._time
	self._time = curTime
	return elapse
end

function Ticker:addTick(tick)
	--[[
	����һ�����㴦��
	@param	tick:
	@type	tick: object of ITick
	--]]
	table.insert(self._ticks, tick)
end

function Ticker:removeTick(tick)
	--[[
	ɾ��һ�����㴦��
	--]]
	table.rmArrayValue(self._ticks, tick)
end

function Ticker:tick()
	--[[
	�߼����㴦��
	--]]
	local elapse = self:_elapse()
	for _, v in ipairs(self._ticks) do
		v:tick(elapse)
	end
end