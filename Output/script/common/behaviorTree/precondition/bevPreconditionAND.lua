--[[Created by Luada
前提条件AND类
--]]

BevPreconditionAND = class(BevPreconditionComp)

function BevPreconditionAND:__init(name, ...)
	BevPreconditionComp.__init(self, name, ...)
end

function BevPreconditionAND:__release()
end

function BevPreconditionAND:test(input)
	bevSearchTracer:enterSearch(self)
	if self.arg_ == nil then
		bevSearchTracer:leaveSearch(self, false)
		return false
	end
	
	for _, v in ipairs(self.arg_) do
		---local nowTime = YXS_PVE.GetElapse()
		if not v:test(input) then
			DEBUG_MSG("BevPreconditionAND test:", v:getName(), false)
			---table.insert(AICostTime, {v:getName(), (YXS_PVE.GetElapse() - nowTime) * 1000})
			bevSearchTracer:process(v:getName(), false)
			bevSearchTracer:leaveSearch(self, false)
			return false
		else
			bevSearchTracer:process(v:getName(), true)
			---table.insert(AICostTime, {v:getName(), (YXS_PVE.GetElapse() - nowTime) * 1000})
		end
		DEBUG_MSG("BevPreconditionAND test:", v:getName(), true)		
	end
	bevSearchTracer:leaveSearch(self, true)
	return true
end
