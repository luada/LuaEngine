--[[Created by Luada
��Ϊ���Ĳ��нڵ�

���Խ׶Σ�
	���ε������е��ӽڵ��test�����������е��ӽڵ㶼����true��������Ҳ����true��
	���򣬷���false
	
����׶Σ�
	���������ӽڵ��tick�����������нڵ��ǡ����ߡ��Ĺ�ϵ����ֻҪ��һ���ӽڵ㷵��
	���н�����������ͷ������н����������нڵ��ǡ����ҡ��Ĺ�ϵ����ֻ�����е��ӽ�
	�㷵�ؽ���������ŷ������н���

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
	���ò��нڵ���߼���ϵ
	@param	logic��
	@type	logic��ExcLogic
	--]]
	self._excLogic = logic
end

function ParallelBevNode:onTest_(input)
	--[[
	���Ժ����Ļص�����
	@param	input��
	@type	input��any
	@param	return��
	@type	return��boolean
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
	�ڵ�ת��ʱ�Ļص�����
	@param	input��
	@type	input��any
	--]]
	for i, v in ipairs(self.childNodes_) do
		self._childNodeStatus[i] = BevRunningStatus.Executing
		v:transition(input)
	end
end

function ParallelBevNode:onTick_(input, output)
	--[[
	��Ϊ���Ĵ���ص�����
	@param	input��
	@type	input��any
	@param	output��
	@type	output��object or table
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
