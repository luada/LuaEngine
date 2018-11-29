--[[Created by Luada
�����ȼ���ѡ��ڵ�

���Խ׶Σ�
	�ӵ�һ���ӽڵ㿪ʼ���α������е��ӽڵ㣬������test�����������ִ��ڿ���
	���е��ӽڵ�ʱ����¼�ӽڵ�������ֹͣ����������true

����׶Σ�
	���ÿ������е��ӽڵ��tick���������������ص�����״̬��Ϊ���������״̬����
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
	���Ժ����Ļص�����
	@param	input��
	@type	input��any
	@param	return��
	@type	return��boolean
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
	�ڵ�ת��ʱ�Ļص�����
	@param	input��
	@type	input��any
	--]]
	if self:checkIndex_(self.lastIdx_) then
		self.childNodes_[self.lastIdx_]:transition(input)
	end
	self.lastIdx_ = -1
end

function PrioritySelectorBevNode:onTick_(input, output)
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