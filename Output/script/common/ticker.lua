--[[Created by Luada
逻辑运算处理模块
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
	获取时间间隔
	@param	return：
	@type	return：float，单位：秒
	--]]
	local curTime = self._getElapse()
	local elapse = curTime - self._time
	self._time = curTime
	return elapse
end

function Ticker:addTick(tick)
	--[[
	加入一个运算处理
	@param	tick:
	@type	tick: object of ITick
	--]]
	table.insert(self._ticks, tick)
end

function Ticker:removeTick(tick)
	--[[
	删除一个运算处理
	--]]
	table.rmArrayValue(self._ticks, tick)
end

function Ticker:tick()
	--[[
	逻辑运算处理
	--]]
	local elapse = self:_elapse()
	for _, v in ipairs(self._ticks) do
		v:tick(elapse)
	end
end