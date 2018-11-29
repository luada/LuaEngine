--[[Created by Luada
前提条件OR类
--]]

BevPreconditionOR = class(BevPreconditionComp)

function BevPreconditionOR:__init(name, ...)
	BevPreconditionComp.__init(self, name, ...)
end

function BevPreconditionOR:__release()
end

function BevPreconditionOR:test(input)
	bevSearchTracer:enterSearch(self)
	for _, v in ipairs(self.arg_ or table.empty) do
		---local nowTime = YXS_PVE.GetElapse()
		if v:test(input) then
			DEBUG_MSG("BevPreconditionOR test:", v:getName(), true)
			bevSearchTracer:process(v:getName(), true)
			bevSearchTracer:leaveSearch(self, true)
			---table.insert(AICostTime, {v:getName(), (YXS_PVE.GetElapse() - nowTime) * 1000})
			return true
		else
			bevSearchTracer:process(v:getName(), false)
			---table.insert(AICostTime, {v:getName(), (YXS_PVE.GetElapse() - nowTime) * 1000})
		end
		DEBUG_MSG("BevPreconditionOR test:", v:getName(), false)
	end
	bevSearchTracer:leaveSearch(self, false)
	return false
end
