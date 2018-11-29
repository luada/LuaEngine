--[[Created by Luada
���Ȩֵѡ��ڵ�;���ӽڵ���WeightBevNode

���Խ׶Σ�
	���ε��������ӽڵ��test����������ΪTrueʱ��������getWeight��������ȡ��ǰ���Ե�Ȩֵ��
	��ѡȡȨֵ�����ӽڵ���Ϊ����ڵ�

����׶Σ�
	����Ȩֵ�����ӽڵ�WeightBevNode��tick����
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
	���Ժ����Ļص�����
	@param	input��
	@type	input��any
	@param	return��
	@type	return��boolean
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
	�ڵ�ת��ʱ�Ļص�����
	@param	input��
	@type	input��any
	--]]
	if self._excNode ~= nil then
		return self._excNode:transition(input)
	end
	return BevRunningStatus.ErrorTransition
end

function WeightSelectorBevNode:onTick_(input, output)
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
	if self._excNode ~= nil then
		excStatus = self._excNode:tick(input, output)
	end
	return excStatus
end