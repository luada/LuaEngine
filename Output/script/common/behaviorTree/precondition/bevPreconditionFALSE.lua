--[[Created by Luada
ǰ������FALSE��
--]]

BevPreconditionFALSE = class(BevPrecondition)

function BevPreconditionFALSE:__init(name)
	BevPrecondition.__init(self, name)
end

function BevPreconditionFALSE:__release()
end

function BevPreconditionFALSE:load()
end

function BevPreconditionFALSE:test(input)
	return false
end
