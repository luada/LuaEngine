--[[Created by Luada
ǰ������TRUE��
--]]

BevPreconditionTRUE = class(BevPrecondition)

function BevPreconditionTRUE:__init(name)
	BevPrecondition.__init(self, name)
end

function BevPreconditionTRUE:__release()
end

function BevPreconditionTRUE:load()
end

function BevPreconditionTRUE:test(input)
	return true
end
