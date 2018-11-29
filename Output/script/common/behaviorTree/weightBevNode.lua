--[[Created by Luada
但权值的节点，权值在test阶段时决定；用作WeightSelectorBevNode的子类
--]]

WeightBevNode = class(BevNode)

function WeightBevNode:__init(name, precondition, weight)
	BevNode.__init(self, name, precondition)
	self._setWeight = weight
	self._curWeight = weight	---当前权值；默认等于初始设定权值，可在测试阶段动态调整
end

function WeightBevNode:__release()
end

function WeightBevNode:getWeight()
	--[[
	获取当前权值
	--]]
	return self._curWeight
end
