--[[Created by Luada
前提条件NOT类
--]]

BevPreconditionNOT = class(BevPrecondition)

function BevPreconditionNOT:__init(name)
	--[[
	@param	name: 条件名字
	@type	name: string
	--]]
	BevPrecondition.__init(self, name)
	self._cnd = nil
end

function BevPreconditionNOT:__release()
	release(self._cnd)
	self._cnd = nil
end

function BevPreconditionNOT:load(...)
	--[[
	--]]
	self._cnd = table.pack(...)[1]
end

function BevPreconditionNOT:test(input)
	---local nowTime = YXS_PVE.GetElapse()
	bevSearchTracer:enterSearch(self)
	if self._cnd == nil then
		bevSearchTracer:leaveSearch(self, false)
		return false
	end

	local ret = not self._cnd:test(input)
	bevSearchTracer:process(self._cnd:getName(), not ret)
	bevSearchTracer:leaveSearch(self, ret)
	---table.insert(AICostTime, {self._cnd:getName(), (YXS_PVE.GetElapse() - nowTime) * 1000})
	return ret
end
