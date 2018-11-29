--[[Created by Luada
行为树的并行节点

测试阶段：
	依次调用所有的子节点的test方法，若所有的子节点都返回true，则自身也返回true，
	否则，返回false
	
处理阶段：
	调用所有子节点的tick方法，若并行节点是“或者”的关系，则只要有一个子节点返回
	运行结束，那自身就返回运行结束。若并行节点是“并且”的关系，则只有所有的子节
	点返回结束，自身才返回运行结束

--]]

ParallelBevNode = class(ComponentBevNode)

function ParallelBevNode:__init(name)
	ComponentBevNode.__init(self, name)
	self._excLogic = ExcLogic.Or
	self._childNodeStatus = {}
end

function ParallelBevNode:__release()
end

function ParallelBevNode:setExcLogic(logic)
	--[[
	设置并行节点的逻辑关系
	@param	logic：
	@type	logic：ExcLogic
	--]]
	self._excLogic = logic
end

function ParallelBevNode:onTest_(input)
	--[[
	测试函数的回调处理
	@param	input：
	@type	input：any
	@param	return：
	@type	return：boolean
	--]]
	for i, v in ipairs(self.childNodes_) do
		local status = self._childNodeStatus[i]
		status = status == nil and BevRunningStatus.Executing or status
		if status == BevRunningStatus.Executing then
			if not v:test(input) then
				return false
			end
		end
	end
	return true
end

function ParallelBevNode:onTransition_(input)
	--[[
	节点转移时的回调处理
	@param	input：
	@type	input：any
	--]]
	for i, v in ipairs(self.childNodes_) do
		self._childNodeStatus[i] = BevRunningStatus.Executing
		v:transition(input)
	end
end

function ParallelBevNode:onTick_(input, output)
	--[[
	行为树的处理回调函数
	@param	input：
	@type	input：any
	@param	output：
	@type	output：object or table
	@param	return:
	@type	return: BevRunningStatus
	--]]
	local finishedChildCount = 0
	local childNodeCount = #self.childNodes_
	for i, v in ipairs(self.childNodes_) do
		local status = self._childNodeStatus[i]
		status = status == nil and BevRunningStatus.Executing or status
		if self._excLogic == ExcLogic.Or then
			if status == BevRunningStatus.Executing then
				 status = v:tick(input, output)
				 self._childNodeStatus[i] = status
			end
			if status ~= BevRunningStatus.Executing then
				for i = 1, childNodeCount do
					self._childNodeStatus[i] = BevRunningStatus.Executing
					return BevRunningStatus.Finish
				end
			end
		else ---And
			if status == BevRunningStatus.Executing then
				status = v:tick(input, output)
				self._childNodeStatus[i] = status
			end
			if status ~= BevRunningStatus.Executing then
				finishedChildCount = finishedChildCount + 1
			end
		end
	end
	
	if finishedChildCount == childNodeCount then
		for i = 1, childNodeCount do
			self._childNodeStatus[i] = BevRunningStatus.Executing
		end
		return BevRunningStatus.Finish
	end
	return BevRunningStatus.Executing
end
