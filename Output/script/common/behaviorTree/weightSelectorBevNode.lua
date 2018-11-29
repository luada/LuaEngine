--[[Created by Luada
最大权值选择节点;其子节点是WeightBevNode

测试阶段：
	依次调用所有子节点的test方法，返回为True时；调用其getWeight方法，获取当前测试的权值；
	并选取权值最大的子节点作为处理节点

处理阶段：
	调用权值最大的子节点WeightBevNode的tick方法
--]]

WeightSelectorBevNode = class(ComponentBevNode)

function WeightSelectorBevNode:__init(name)
	ComponentBevNode.__init(self, name)
	self._excNode = nil
end

function WeightSelectorBevNode:__release()
end

function WeightSelectorBevNode:onTest_(input)
	--[[
	测试函数的回调处理
	@param	input：
	@type	input：any
	@param	return：
	@type	return：boolean
	--]]
	local maxW = -1
	for _, v in ipairs(self.childNodes_) do
		if v:test(input) then
			local curW = v:getWeight()
			if curW > maxW then
				maxW = curW
				self._excNode = v
			end
		end
	end
	return self._excNode ~= nil
end

function WeightSelectorBevNode:onTransition_(input)
	--[[
	节点转移时的回调处理
	@param	input：
	@type	input：any
	--]]
	if self._excNode ~= nil then
		return self._excNode:transition(input)
	end
	return BevRunningStatus.ErrorTransition
end

function WeightSelectorBevNode:onTick_(input, output)
	--[[
	行为树的处理回调函数
	@param	input：
	@type	input：any
	@param	output：
	@type	output：object or table
	@param	return:
	@type	return: BevRunningStatus
	--]]
	local excStatus = BevRunningStatus.Finish
	if self._excNode ~= nil then
		excStatus = self._excNode:tick(input, output)
	end
	return excStatus
end