--[[Created by Luada
��Ϊ����ѭ���ڵ�

���Խ׶Σ�
	Ԥ���ѭ���������˾ͷ���false������ֻ���õ�һ���ӽڵ��test������
	���������ص�ֵ��Ϊ�����ֵ����
	
����׶Σ�
	ֻ���õ�һ���ڵ��tick���������������н��������Ƿ���Ҫ�ظ����У�
	��ѭ������û�����������������У���ѭ�������ѵ����򷵻����н���

--]]

LoopBevNode = class(ComponentBevNode)

function LoopBevNode:__init(name)
	ComponentBevNode.__init(self, name)
	self._loopCount = loopCount		---Ϊ-1ʱ����ʾ���޴ε���
	self._currentCount = 0
end

function LoopBevNode:__release()
end

function LoopBevNode:onTest_(input)
	--[[
	���Ժ����Ļص�����
	@param	input��
	@type	input��any
	@param	return��
	@type	return��boolean
	--]]
	local check = self._loopCount == -1 or self._currentCount < self._loopCount
	if not check then
		return false
	end
	
	if self:checkIndex_(1) then
		return self.childNodes_[1]:test(input)
	end
	
	return false
end

function LoopBevNode:onTransition_(input)
	--[[
	�ڵ�ת��ʱ�Ļص�����
	@param	input��
	@type	input��any
	--]]
	if self:checkIndex_(1) then
		self.childNodes_[1]:transition(input)
	end
	self._currentCount = 0
end

function LoopBevNode:onTick_(input, output)
	--[[
	��Ϊ���Ĵ���ص�����
	@param	input��
	@type	input��any
	@param	output��
	@type	output��object or table
	@param	return:
	@type	return: BevRunningStatus
	--]]
	local excStatus = BevRunningStatus.Finish
	if self:checkIndex_(1) then
		excStatus = self.childNodes_[1]:tick(input, output)
	end
	if excStatus == BevRunningStatus.Finish then
		if self._loopCount ~= -1 then
			self._currentCount = self._currentCount + 1
			if self._currentCount ~= self._loopCount then
				excStatus = BevRunningStatus.Executing
			end
		else
			excStatus = BevRunningStatus.Executing
		end
	end
	
	if excStatus ~= BevRunningStatus.Executing then
		self._currentCount = 0
	end
	return excStatus
end