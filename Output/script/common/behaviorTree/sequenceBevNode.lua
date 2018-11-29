--[[Created by Luada
��Ϊ�������нڵ�

���Խ׶Σ�
	���Ǵ�ͷ��ʼ�ģ�����õ�һ���ӽڵ��test���������䷵��ֵ��Ϊ����ķ���ֵ���ء�
	���򣬵��õ�ǰ���нڵ��test���������䷵��ֵ��Ϊ����ķ���ֵ���ء�

����׶Σ�
	���ÿ������е��ӽڵ��tick���������������н���������һ���ӽڵ���Ϊ��ǰ���нڵ㣬
	����ǰ�������һ���ӽڵ㣬��ʾ�������Ѿ����н����������������н��������ӽڵ㷵
	�������У������������ص�����״̬��Ϊ���������״̬����
--]]

SequenceBevNode = class(ComponentBevNode)

function SequenceBevNode:__init(name)
	ComponentBevNode.__init(self, name)
	self._curIdx = -1
end

function SequenceBevNode:__release()
end

function SequenceBevNode:onTest_(input)
	--[[
	���Ժ����Ļص�����
	@param	input��
	@type	input��any
	@param	return��
	@type	return��boolean
	--]]
	local testIdx = self._curIdx == -1 and 1 or self._curIdx
	if self:checkIndex_(testIdx) then
		if self.childNodes_[testIdx]:test(input) then
			return true
		end
	end
	return false
end

function SequenceBevNode:onTransition_(input)
	--[[
	�ڵ�ת��ʱ�Ļص�����
	@param	input��
	@type	input��any
	--]]
	if self:checkIndex_(self._curIdx) then
		self.childNodes_[self._curIdx]:transition(input)
	end
	self._curIdx = -1
end

function SequenceBevNode:onTick_(input, output)
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
	local curIdx = self._curIdx == -1 and 1 or self._curIdx
	if self:checkIndex_(curIdx) then
		excStatus = self.childNodes_[curIdx]:tick(input, output)
		if excStatus == BevRunningStatus.Finish then
			self._curIdx = curIdx + 1
			if self._curIdx >= #self.childNodes_ then
				self._curIdx = -1
			else
				excStatus = BevRunningStatus.Executing
			end
		elseif excStatus < 0 then	---error occurred
			self._curIdx = -1
		end
	end
	return excStatus
end
