--[[Writen by Luada

常用的函数

Exported API:
	define(class, table)
	dobuffer(strCode)
	smartImport(name, path, autoRelease)
	unpackArgs(...)
	getGlobalName(object)
	getObjectKey(object)
	getObjectsKey(...)
	toNumber(value, default)
	toBool(value)

	string.startsWith(value, prefix, toffset)
	string.endsWith(value, suffix)
	string.title(value)
	string.ltitle(value)
	string.split(s, seg)
	string.charAt(value, position)
	string.isWhitespace(value)
	string.toArray(value)
	string.printf(...)
	string.substr(value, index, length)

	table.copy(source, destiny, overlay)
	table.deepCopy(source, destiny，replace)
	table.copyTable(source, destiny)
	table.join(...)
	table.clear(tbl)
	table.removeValue(tbl, value)
	table.rmArrayValue(tbl, value)
	table.rmMapValue(tbl, value)
	table.removeKey(tbl, key)
	table.hasValue(tbl, value)
	table.hasKey(tbl, key)
	table.size(tbl)
	table.iterator(tbl)
	table.randomIterator(tbl)
	table.sortIterator(tbl, comparator)
	table.contains(tbl, object)
	table.isArray(tbl)
	table.isMap(tbl)
	table.unpack(tbl)
	table.pack(...)
	table.getKey(tbl, value)
	table.getKeyName(tbl, value)
	

	math.rand(n1, n2)
	math.round(n)

Example:
	local classMap = define { __mode = "k" }

	for k,v in table.iterator(t) do
		print(k,v)
	end
--]]


--------------------------------------------------------------------------------
function define(class, object)
	--[[
	为object绑定一个元表；当object为nil时，则新建一个表
	@param	class: 元表
	@type	class: table or class
	@param	object: 要绑定元表的对象
	@type	object: table or class or nil
	@param	return: 返回被绑定元表的object
	@type	return: table or class
	--]]
	return setmetatable(object or {}, class)
end
--------------------------------------------------------------------------------
function dobuffer(strCode)
	--[[
	解析运行源代码，当解析运行有误时，回抛出AssertException
	@param	strCode: 源代码字符串
	@type	strCode： string
	@param	return：返回执行结果
	@type	return：视具体的源代码而定
	--]]
	local status, ret = pcall(loadstring(strCode))
	assert(status, ret)
	return ret
end
--------------------------------------------------------------------------------
function smartImport(name, path, autoRelease)
	--[[
	加载模块，并返回相应的值
	@param	name：模块名字
	@type	name：string or nil
	@param	path: 模块文件相对路径
	@type	path：string or nil
	@param	autoRelease: 是否自动释放内部引用
	@type	autoRelease: boolean or nil
	@param	return: module{class or function or table or nil}
	@type	return: 视module而定
	--]]
	if path ~= nil then
		if autoRelease then
			package.loaded[path] = nil
		end
		require(path)
	end
	
	if name ~= nil then
		if autoRelease then
			local code = string.format("do local _ = %s \n %s = nil return _ end", name, name)
			local ret = dobuffer(code)
			package.loaded[path] = nil
			return ret
		else
			local code = string.format("do local _ = %s return _ end", name)
			return dobuffer(code)
		end
	end
	return nil
end
--------------------------------------------------------------------------------
function unpackArgs(...)
	--[[
	将所有arg解包
	@param	arg：由arg组成的arg
	@type	arg：(arg1，arg2， ..., count)
	@param	return: arg解包后的参数列表
	@type	return：参数列表
	--]]
	local idx = 0
	local ret = {}
	for i = 1, select("#", ...) do
		local v = select(i, ...)
	    local size = arraySize(v)
	    if size == nil then
	        idx = idx + 1
	        ret[idx] = v
	    else
	        for i = 1, size do
	            idx = idx + 1
	            ret[idx] = v[i]
            end
	    end
	end
	return unpackArray(ret)
end
--------------------------------------------------------------------------------
function getGlobalName(object)
	--[[
	在全局table(_G)中查找object的名字
	@param	object: 任一待查的lua值
	@type	object: any
	@param	return: object的名字
	@type	return: string
	--]]
	if object then
		for k, v in pairs(_G) do
			if v == object then
				return k
			end
		end
	end
	return tostring(nil)
end
--------------------------------------------------------------------------------
function getObjectKey(object)
	--[[
	获取object的键值：
	（1）object为table时，返回table的内存地址
	（2）为nil时，返回""
	（3）其它情况返回tostring(object)
	@param	return: object的key值
	@type	return: string
	--]]
	local _, _, key = string.find(tostring(object), ":%s*(%w+)")
	return key or (object or tostring(object) and "")
end
--------------------------------------------------------------------------------
function getObjectsKey(...)
	--[[
	获取输入参数的key值，key值间用"_"连接
	@param	arg: 任意参数
	@type	arg: any
	@param	return: _"key0"_"key1"_"key2"_...
	@type	return: string
	--]]
	local ret = ""
	for i = 1, select("#", ...) do
		local v = select(i, ...)
		ret = string.format("%s_%s", ret, getObjectKey(v))
	end
	return ret
end
--------------------------------------------------------------------------------
function toNumber(value, default)
	--[[
	将value转为数字类型; 转换失败时,使用default值
	@param	value: 任意值
	@type	value: any
	@param	default: 默认值
	@type	default：number
	@param	return: 转换结果
	@type	return: number
	--]]
	local ret = tonumber(value)
	return ret and ret or (default or 0)
end
--------------------------------------------------------------------------------
function toBool(value)
	--[[
	将value转为bool类型;返回true的情况:value值为true, "true", "yes", "y"， 大于0的number
	@param	return: 转换结果
	@type	return: boolean(true or false)
	--]]
	if type(value) == "boolean" then
		return value
	elseif type(value) == "string" then
		local str = string.lower(value)
		if str == "true" or str == "yes" or str == "y" then
			return true
		else
			return false
		end
	elseif type(value) == "number" then
		if value > 0 then
			return true
		else
			return false
		end
	else
		return false
	end
end
--------------------------------------------------------------------------------
function string.startsWith(value, prefix, offset)
	--[[
	判断字符串value从offset偏移量处是否以prefix字符串开始
	@param	value: 被测试的字符串
	@type	value: string
	@param	prefix: 测试的字符串
	@type	prefix:	string
	@param	offset: 从被测试字符串的左边开始的偏移量,左边第一个偏移量为1
	@type	offset: number
	@param	return: 测试结果
	@type	return: boolean(true or false)
	--]]
	if value and prefix then
		offset = (offset or 1) > 0 and offset or 1
		return string.sub(value, offset, offset + #prefix - 1) == prefix
	end
	return false
end
--------------------------------------------------------------------------------
function string.endsWith(value, suffix)
	--[[
	判断字符串value是否以suffix字符串结束
	@param	value: 被测试的字符串
	@type	value: string
	@param	suffix: 测试字符串
	@type	suffix: string
	@param	return: 测试结果
	@type	return: boolean(true or false)
	--]]
	if value and suffix then
		return string.sub(value, -#suffix) == suffix
	end
	return false
end
--------------------------------------------------------------------------------
function string.title(value)
	--[[
	将字符串value左边第一个字母变为大写，通常在property或者prototype中设置函数名时用到
	@param	value: 
	@type	value：string
	@param	return: value的标题
	@type	return: string
	--]]
	return string.upper(string.sub(value, 1, 1)) .. string.sub(value, 2, #value)
end
--------------------------------------------------------------------------------
function string.ltitle(value)
	--[[
	将字符串value左边第一个字母变为小写
	@param	value: 
	@type	value：string
	@param	return: value的标题
	@type	return: string
	--]]
	return string.lower(string.sub(value, 1, 1)) .. string.sub(value, 2, #value)
end
--------------------------------------------------------------------------------
function string.split(s, seg)
	--[[
	将字符串s按照分隔符为seg,分割保存到一个table
	@param	s：待分割的源字符串
	@type	s：string
	@param	seg：分割字符串
	@type	seg：string
	@param	return：分割结果
	@type	return：table array{string}
	--]]
	local start = 1
	local t = {}
	local segLen = string.len(seg)
	if segLen > 0 then
		while true do
			local pos = string.find(s, seg, start, true)
			if not pos then
				break
			end
			table.insert(t, string.sub(s, start, pos - 1))
			start = pos + segLen
		end
		table.insert(t, string.sub(s, start))
	end
	return t
end
--------------------------------------------------------------------------------
function string.charAt(value, pos)
	--[[
	获取字符串pos位置处的字符。pos为正时，从左到右，1开始递增；为负时，从右到左，-1开始递减
	@param	value：
	@type	value：string
	@param	pos：
	@type	pos：number
	@param	return：字符串pos位置处的字符；pos超出value范围时返回nil
	@type	return：string
	--]]
	pos = pos > 0 and pos or #value + 1 + pos
	if pos <= 0 or not value then
		return nil
	end
	local b = string.byte(value, pos, pos + 1)
	return b and string.char(b) or b
end
--------------------------------------------------------------------------------
function string.isWhitespace(value)
	--[[
	判断value是否全部由空格(" ")或者制表符("\t")组成
	@param	value:
	@type	value: string
	@param	return: 判断结果
	@type	return: boolean(true or false)
	--]]
	if value then
		local len = #value
		for i = 1, len do
			local char = string.charAt(value, i)
			if char ~= " " and char ~= "\t" then
				return false
			end
		end
		return true
	end
	return false
end
--------------------------------------------------------------------------------
function string.printf(...)
	--[[
	将可变参数字符串化
	@param	arg：
	@type	arg：
	@param	return：
	@type	return：string
	--]]
	local t = {}
	for i = 1, select("#", ...) do
		table.insert(t, tostring(select(i, ...)))
	end

	local str = string.gsub(table.concat(t, "  "), "%%", "%%%%")
	
	return str
end
--------------------------------------------------------------------------------
function string.toArray(value)
	--[[
	将字符串value按照字符拆开,存放在新建的表里面
	@param	value: 
	@type	value: string
	@param	return: 存放字符串value的表
	@type	return: table
	--]]
	local ret = {}
	if value then
		local idx = 1
		local count = #value
		while idx <= count do
			local b = string.byte(value, idx, idx + 1)
			if b > 127 then
				table.insert(ret, string.sub(value, idx, idx + 1))
				idx = idx + 2
			else
				table.insert(ret, string.char(b))
				idx = idx + 1
			end
		end
	end
	return ret
end
--------------------------------------------------------------------------------
function string.substr(value, pos, length)
	--[[
	从字符串value中获取pos位置开始，长度为length的子字符串
	@param	value：原字符串
	@type	value：string
	@param	pos：从左到右，以1开始的要获得子字符串的位置
	@type	pos：number
	@param	length：要获得子字符串的长度
	@type	length：number
	@param	return：子字符串
	@type	return：string
	--]]
	if value then
		local ret = {}
		local idx = pos
		local count = length or #value
		while idx <= count do
			local b = string.byte(value, idx, idx + 1)
			if not b then
				break
			end
			if b > 127 then
				table.insert(ret, string.sub(value, idx, idx + 1))
				idx = idx + 2
			else
				table.insert(ret, string.char(b))
				idx = idx + 1
			end
		end
		return table.concat(ret)
	end
end
--------------------------------------------------------------------------------
table.empty = define({ __newindex = function(s, f, v) assert(false, "This is a read-only table!") end }, {})
--------------------------------------------------------------------------------
function table.copy(source, destiny, overlay)
	--[[
	将源表(source)浅度copy到目标表(destiny);当且仅当overlay为false时,不覆盖destiny原有的值
	@param	source: 要拷贝的源表
	@type	source: table
	@param	destiny: 存放结果的表,当此值为nil时,会新建一个表
	@type	destiny: table or nil
	@param	overlay: 标志是否覆盖目标表原有的值(默认覆盖）
	@type	overlay: boolean(true or false)
	@param	return: 返回浅度拷贝的表;当destiny为nil时,返回存放结果的新表,否则返回destiny表
	@type	return: table
	--]]
	if source then
		overlay = overlay ~= false
		if not destiny then destiny = {} end
		for field, value in pairs(source) do
			if overlay then
				destiny[field] = value
			elseif not destiny[field] then
				destiny[field] = value
			end
		end
	end
	return destiny
end
--------------------------------------------------------------------------------
function table.deepCopy(source, destiny, replace)
	--[[
	深度copy
	@param	source: 要拷贝的源表
	@type	source: table
	@param	destiny: 存放结果的表,当此值为nil时,会新建一个表
	@type	destiny: table or nil
	@param	replace: 是否替换原来的值; 为false或者nil时，使用table.insert,源key不再保留,copy数据
	@type	replace: boolean or nil
	@param	return: 返回深度拷贝的表;当destiny为nil时,返回存放结果的新表,否则返回destiny表
	@type	return: table
	--]]
	local destiny = destiny or {}
	for key, value in pairs(source or table.empty) do
		if not replace and destiny[key] then
			table.insert(destiny,value)
		else
			if type(value) == "table" then
				destiny[key] = table.deepCopy(value)
			else
				destiny[key] = value
			end
		end
	end
	return destiny
end
--------------------------------------------------------------------------------
function table.copyTable(source, destiny)
	--[[
	把一个table复制到另一个, 不会保留key值，但目标table不会受到影响
	@param	source: 要拷贝的源表
	@type	source: table
	@param	destiny: 存放结果的表,当此值为nil时,会新建一个表
	@type	destiny: table or nil
	@param	return: 返回深度拷贝的表;当destiny为nil时,返回存放结果的新表,否则返回destiny表
	@type	return: table
	--]]
	local destiny = destiny or {}
	for _, value in pairs(source or table.empty) do
		if type(value) == "table" then
			table.insert(destiny, table.copyTable(value))
		else
			table.insert(destiny, value)
		end
	end
	return destiny
end
--------------------------------------------------------------------------------
function table.join(...)
	--[[
	将由table组成的参数列表(arg)连接到新建的表,忽略源表的key值
	@param arg: 由table组成的参数列表
	@type arg: {table, table, ...}
	@param	return: 参数列表(arg)连接成的新表
	@type	return: table
	--]]
	local ret = {}
	for i = 1, select("#", ...) do
		local tb = select(i, ...)
		for _, value in pairs(tb or table.empty) do
			table.insert(ret, value)
		end
	end
	return ret
end
--------------------------------------------------------------------------------
function table.clear(tbl)
	--[[
	将表tbl清空
	@param	tbl：待清空的表
	@type	tbl：table
	@param	return：清空后的tbl表
	@type	return：table
	--]]
	if type(tbl) == "table" then
		local field = next(tbl)
		while field do
			table.clear(tbl[field])
			tbl[field] = nil
			field = next(tbl)
		end
	end
	return tbl
end
--------------------------------------------------------------------------------
function table.rmArrayValue(tbl, value)
	--[[
	在数组表tbl中移去value的值
	@param	tbl：数组源表
	@type	tbl：table
	@param	value：将要移去的值
	@type	value：any
	@param	return：移去value值后的tbl
	@type	return：table
	--]]
	local idx = 1
	for k, v in pairs(tbl or table.empty) do
		if v == value then
			table.remove(tbl, idx)
			break
		end
		idx = idx + 1
	end
	return tbl
end
--------------------------------------------------------------------------------
function table.rmMapValue(tbl, value)
	--[[
	在Map表tbl中移去value的值
	@param	tbl：Map源表
	@type	tbl：table
	@param	value：将要移去的值
	@type	value：any
	@param	return：移去value值后的tbl
	@type	return：table
	--]]
	for k, v in pairs(tbl or table.empty) do
		if v == value then
			tbl[k] = nil
			break
		end
	end
	return tbl
end
--------------------------------------------------------------------------------
function table.removeValue(tbl, value)
	--[[
	在表tbl中移去value的值
	@param	tbl：源表
	@type	tbl：table
	@param	value：将要移去的值
	@type	value：any
	@param	return：移去value值后的tbl
	@type	return：table
	--]]
	if tbl then
		if table.isArray(tbl) then
			table.rmArrayValue(tbl, value)
		else
			table.rmMapValue(tbl, value)
		end
	end
	return tbl
end
--------------------------------------------------------------------------------
function table.removeKey(tbl, key)
	--[[
	从表tbl中移去键值为key的键和值
	@param	tbl：源表
	@type	tbl：table
	@param	key：将要移去的键值
	@type	key：string or number
	@param	return：移去键值key后的tbl
	@type	return：table
	--]]
	if tbl then
		for k, v in pairs(tbl) do
			if k == key then
				tbl[k] = nil
				break
			end
		end
	end
	return tbl
end
--------------------------------------------------------------------------------
function table.hasValue(tbl, value)
	--[[
	测试表tbl是否存在值value
	@param	tbl: 源表
	@type	tbl: table
	@param	key：要测试的值
	@type	key：any
	@param	return：测试结果
	@type	return：boolean(true or false)
	--]]
	for k, v in pairs(tbl) do
		if v == value then
			return true
		end
	end
	return false
end
--------------------------------------------------------------------------------
function table.hasKey(tbl, key)
	--[[
	测试表tbl是否存在键key
	@param	tbl:源表
	@type	tbl:table
	@param	key:要测试的键
	@type	key: number or string
	@param	return: 测试结果
	@type	return: boolean(true or false)
	--]]
	for k, v in pairs(tbl, key) do
		if k == key then
			return true
		end
	end
	return false
end
--------------------------------------------------------------------------------
function table.size(tbl)
	--[[
	求表tbl的长度;相当于求表长度的操作符#
	@param	tbl: 源表
	@type	tbl: table
	@param	return: 表长度
	@type	return: number
	--]]
	local size = 0
	if tbl then
		table.foreach(tbl, function()
			size = size + 1
		end)
	end
	return size
end
--------------------------------------------------------------------------------
local emptyFunc = function() end
function table.iterator(tbl)
	--[[
	获取访问表tbl的迭代函数
	@param	tbl: 源表
	@type	tbl: table
	@param	return: 访问tbl的迭代函数
	@type	return: function 
	--]]
	if tbl then
		local index = 0
		local auxTable = {}
		table.foreach(tbl, function(i, v)
			if type(i) == "number" then
				table.insert(auxTable, i)
			else
				table.insert(auxTable, tostring(i))
			end
		end)

		return function()
			if index < #auxTable then
				index = index + 1
				local field = auxTable[index]
				return field, tbl[field]
			end
		end
	else
		return emptyFunc
	end
end
--------------------------------------------------------------------------------
function table.randomIterator(tbl)
	--[[
	获取访问表tbl的随机迭代函数
	@param	tbl: 源表,array方式
	@type	tbl: table
	@param	return: 访问tbl的迭代函数
	@type	return: function 
	--]]
	if tbl then
		local count = #tbl
		local idxs = {}
		for i = 1, count do
			local idx = math.random(1, i)
			table.insert(idxs, idx, i)
		end
		
		local idx = 0
		return 	function()
			if idx < count then
				idx = idx + 1
				local k = idxs[idx]
				return k, tbl[k]
			end
		end
	else
		return emptyFunc
	end
end
--------------------------------------------------------------------------------
function table.sortIterator(tbl, comparator)
	--[[
	获取被comparator排序过的访问tbl的迭代函数
	@param	tbl: 源表
	@type	tbl：table
	@param	comparator: 比较函数; 例子：迭代函数返回递增数列时function(a, b) return a < b end
	@type	comparator: function(a, b)
	@param	return: 被comparator排序过的访问tbl的迭代函数
	@type	return: function 
	--]]
	if tbl then
		local index = 0
		local auxTable = {}
		table.foreach(tbl, function(i, v)
			table.insert(auxTable, i)
		end)

		table.sort(auxTable, comparator)

		return function()
			if index < #auxTable then
				index = index + 1
				local field = auxTable[index]
				return field, tbl[field]
			end
		end
	else
		return emptyFunc
	end
end
--------------------------------------------------------------------------------
function table.contains(tbl, object)
	--[[
	测试源表是否包含object
	@param	tbl： 源表
	@type	tbl：table
	@param	object：测试值
	@type	object：any
	@param	return：测试结果
	@type	return：boolean（true or false）
	--]]
	if tbl and object then
		for field, value in pairs(tbl) do
			if object == value then return true end
		end
	end
	return false
end
--------------------------------------------------------------------------------
function table.include(tbl,element)
	--[[
	测试表tbl是否包含某个元素element,支持数组(必须k,v同时相等)
	@param	tbl： 源表
	@type	tbl：table
	@param	element：测试元素
	@type	element：any
	@param	return：测试结果
	@type	return：boolean（true or false）
	--]]
	for k,v in pairs(tbl or table.empty) do
		local done=false
		if type(v)=="table" and type(element)=="table" then
			done=true
			if table.size(element)~=table.size(v) then
				done=false
			end
			for k2,v2 in pairs(element) do
				if not v[k2] or v[k2]~=v2 then
					done=false
					break
				end
			end
		elseif v==element then
			done=true
		end
		if done then
			return true
		end
	end
	return false
end
----------------------------------------------------------------------------------
function table.includes(tbl,elements)
	--[[
	测试表tbl是否包含一组元素
	@param	tbl： 源表
	@type	tbl：table
	@param	elements：测试元素组
	@type	elements：table
	@param	return：测试结果
	@type	return：boolean（true or false）
	--]]
	for k,v in pairs(elements or table.empty) do
		if not table.include(tbl,v) then
			return false
		end
	end
	return true
end
--------------------------------------------------------------------------------
function table.isArray(tbl)
	--[[
	判断表tbl的键值key是否全由数字组成
	@param	tbl：源表
	@type	tbl：table
	@param	return：测试结果
	@type	return：boolean（true or false）
	--]]
	if not tbl then
		return false
	end

	local ret = true
	local idx = 1
	for f, v in pairs(tbl) do
		if type(f) == "number" then
			if f ~= idx then
				ret = false
			end
		else
			ret = false
		end
		if not ret then break end
		idx = idx + 1
	end
	return ret
end
--------------------------------------------------------------------------------
function table.isMap(tbl)
	--[[
	测试表tbl是否为map；即tbl的key不全由number组成
	@param	tbl：源表
	@type	tbl：table
	@param	return：测试结果
	@type	return：boolean（true or false）
	--]]
	if not tbl then
		return false
	end
	return table.isArray(tbl) ~= true
end
--------------------------------------------------------------------------------
function table.unpack(tbl)
	--[[
	将tbl解包为参数列表
	@param	tbl：源表
	@type	tbl：table
	@param	return：解包后的参数列表
	@type	return: arg
	--]]
	local ret
	if table.isArray(tbl) then
		ret = tbl
	else
		ret = {}
		local idx = 1
		for f, v in pairs(tbl or table.empty) do
			if type(f) == "number" and f == idx then
				ret[f] = v
				idx = idx + 1
			else
				table.insert(ret, {[f]=v})
			end
		end
	end
	return unpack(ret)
end
--------------------------------------------------------------------------------
function table.pack(...)
	--[[
	将可变参数打包为一个table(不用arg，兼容Lua5.1新标准)
	@param	arg: 可变参数列表
	@type	arg: {}
	@param	return: 
	@type	return: table
	--]]
	local arg = {}
	for i = 1, select("#", ...) do
		local v = select(i, ...)
		table.insert(arg, v)
	end
	return arg
end
--------------------------------------------------------------------------------
function table.getKey(tbl, value)
	--[[
	在表tbl中获取值为value的键
	@param	tbl: 源表
	@type	tbl: table
	@param	value: 要测试的值
	@type	value: any
	@param	return: 键
	@type	return：string or int or nil
	--]]
	for k, v in pairs(tbl or table.empty) do
		if v == value then
			return k
		end
	end
	return nil
end
--------------------------------------------------------------------------------
function table.getKeyName(tbl, value)
	--[[
	在表tbl中获取值为value的键名
	@param	tbl: 源表
	@type	tbl: table
	@param	value: 要测试的值
	@type	value: any
	@param	return: 键值
	@type	return：string
	--]]
	return tostring(table.getKey(tbl, value) or "")
end
--------------------------------------------------------------------------------
function table.save(tbl,fname,name)
	--[[
	将表tbl以name的名字保存到文件fname中
	@param	tbl: 源表
	@type	tbl：table
	@param	fname：文件名
	@type	fname：string
	@param	name：表名
	@type	name：string
	@param	return：返回保存结果；保存成功时返回true，失败返回false
	@type	return：boolean(true or false)
	--]]
	if type(tbl) == "table" then
		local f = io.open(fname, "w")
		if f then
			if type(name) == "string" then
				f:write(name.." = ")
			end
			f:write(serialize(tbl))
			f:close()
			return true
		end
	end
	return false
end
--------------------------------------------------------------------------------
function math.rand(n1, n2)
	--[[
	返回n1~n2间的随机数
	@param	n1:
	@type	n1: number
	@param	n2:
	@param	n2: number
1	@param	return: n1~n2间的随机数
	@type	return: number
	--]]
	return math.random(n1*10000, n2*10000)/10000
end
--------------------------------------------------------------------------------
function math.round(n)
	--[[
	eg.
	（1）math.round(-4.5) ： -4
	（2）math.round(4.5)：	5
	@param	n：
	@type	n：number
	@param	return:
	@type	return：int
	--]]
	return math.floor(0.5 + n)
end
