--[[Created by Luada
��Ϊ����ǰ��������
--]]

BevPrecondition = class()

function BevPrecondition:__init(name) 
	--[[
	��ʼ��
	@param	name: �����������
	@type	name: string
	--]]
	self._objName = name
end

function BevPrecondition:getName()
	--[[
	��ȡ����
	@param	return��
	@type	return��string
	--]]
	return self._objName
end

function BevPrecondition:__release()
end

function BevPrecondition:load(...)
end

function BevPrecondition:test(input)
end
