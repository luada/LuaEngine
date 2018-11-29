--[[Created by Luada
不带优先级的选择节点

测试阶段：
	先调用上一个运行的子节点（若存在）的test方法，如果可以运行，则继续运保存该节点
	的索引，返回true，如果不能运行，则重新选择（同带优先级的选择节点的选择方式）

处理阶段：
	调用可以运行的子节点的Tick方法，用它所返回的运行状态作为自身的运行状态返回
--]]
NonePrioritySelectorBevNode = class(PrioritySelectorBevNode)

function NonePrioritySelectorBevNode:__init(name)
	PrioritySelectorBevNode.__init(self, name)
end

function NonePrioritySelectorBevNode:__release()
end

function NonePrioritySelectorBevNode:onTest_(input)
	--[[
	测试函数的回调处理
	@param	input：
	@type	input：any
	@param	return：
	@type	return：boolean
	--]]
	if self:checkIndex_(self.curIdx_) then
		if self.childNodes_[self.curIdx_]:test(input) then
			return true
		end
	end
	return PrioritySelectorBevNode.onTest_(self, input)
end
