--[[Created by Luada
带优先级的选择节点

测试阶段：
	从第一个子节点开始依次遍历所有的子节点，调用其test方法，当发现存在可以
	运行的子节点时，记录子节点索引，停止遍历，返回true

处理阶段：
	调用可以运行的子节点的tick方法，用它所返回的运行状态作为自身的运行状态返回
--]]

PrioritySelectorBevNode = class(ComponentBevNode)

function PrioritySelectorBevNode:__init(name)
	ComponentBevNode.__init(self, name)
	self.lastIdx_ = -1
	self.curIdx_ = -1
end

function PrioritySelectorBevNode:__release()
end

function PrioritySelectorBevNode:onTest_(input)
	--[[
	测试函数的回调处理
	@param	input：
	@type	input：any
	@param	return：
	@type	return：boolean
	--]]
	self.curIdx_ = -1
	for k, v in ipairs(self.childNodes_) do
		if v:test(input) then
			self.curIdx_ = k
			return true
		end
	end
	return false
end

function PrioritySelectorBevNode:onTransition_(input)
	--[[
	节点转移时的回调处理
	@param	input：
	@type	input：any
	--]]
	if self:checkIndex_(self.lastIdx_) then
		self.childNodes_[self.lastIdx_]:transition(input)
	end
	self.lastIdx_ = -1
end

function PrioritySelectorBevNode:onTick_(input, output)
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
	if self:checkIndex_(self.curIdx_) then
		if self.curIdx_ ~= self.lastIdx_ then
			if self:checkIndex_(self.lastIdx_) then
				self.childNodes_[self.lastIdx_]:transition(input)
			end
			self.lastIdx_ = self.curIdx_
		end
	end
	
	if self:checkIndex_(self.lastIdx_) then
		excStatus = self.childNodes_[self.lastIdx_]:tick(input, output)
		if excStatus == BevRunningStatus.Finish then
			self.lastIdx_ = -1
		end
	end
	return excStatus
end