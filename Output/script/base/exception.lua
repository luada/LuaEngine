--[[Writen by Luada

异常处理类

--]]

--所有异常的基类
Exception = class()

local prop = Property(Exception)
prop:reader("msg")
prop:reader("trace")
prop:reader("cause")


function Exception:__init(msg, ex, level)
	local this = prop[self]
	this.msg = msg
	this.trace = debug.traceback()
	this.cause = instanceof(ex, Exception) and ex:getCause() or debug.getinfo(level or 4, "Sl")
end

function Exception:getMessage()
	local this = prop[self]
	return tostring(this.msg and this.msg or "")
end

function Exception:getCauseMessage()
	local cause = self:getCause()
	if cause then
		return string.format("[%s]:%d ->%s", tostring(cause.short_src), toNumber(cause.currentline, -1), self:getMessage())
	end
	return ""
end

function Exception:printStackTrace()
	WARN_MSG(self.trace)
end

function Exception:tostring()
	return string.format("Exception: %s", self:getCauseMessage())
end

--------------------------------------------------------------------------------
--运行时异常
RuntimeException = class(Exception)

function RuntimeException:__init(msg, ex)
	Exception.__init(self, msg, ex, 5)
end

function RuntimeException:tostring()
	return string.format("RuntimeException: %s", self:getCauseMessage())
end

--------------------------------------------------------------------------------
--IO操作异常
IOException = class(Exception)

function IOException:__init(msg, ex)
	Exception.__init(self, msg, ex, 5)
end

function IOException:tostring()
	return string.format("IOException: %s", self:getCauseMessage())
end

--------------------------------------------------------------------------------
--assert判断条件不成立异常
AssertException = class(Exception)

function AssertException:__init(msg, ex)
	Exception.__init(self, msg, ex, 5)
end

function AssertException:tostring()
	return string.format("AssertException: %s", self:getCauseMessage())
end
