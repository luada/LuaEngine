--[[Created by Luada
��Ȩֵ�Ľڵ㣬Ȩֵ��test�׶�ʱ����������WeightSelectorBevNode������
--]]

WeightBevNode = class(BevNode)

function WeightBevNode:__init(name, precondition, weight)
	BevNode.__init(self, name, precondition)
	self._setWeight = weight
	self._curWeight = weight	---��ǰȨֵ��Ĭ�ϵ��ڳ�ʼ�趨Ȩֵ�����ڲ��Խ׶ζ�̬����
end

function WeightBevNode:__release()
end

function WeightBevNode:getWeight()
	--[[
	��ȡ��ǰȨֵ
	--]]
	return self._curWeight
end
