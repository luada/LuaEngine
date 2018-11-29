--[[Writen by Luada

���õĺ���

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
	table.deepCopy(source, destiny��replace)
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
	Ϊobject��һ��Ԫ����objectΪnilʱ�����½�һ����
	@param	class: Ԫ��
	@type	class: table or class
	@param	object: Ҫ��Ԫ��Ķ���
	@type	object: table or class or nil
	@param	return: ���ر���Ԫ���object
	@type	return: table or class
	--]]
	return setmetatable(object or {}, class)
end
--------------------------------------------------------------------------------
function dobuffer(strCode)
	--[[
	��������Դ���룬��������������ʱ�����׳�AssertException
	@param	strCode: Դ�����ַ���
	@type	strCode�� string
	@param	return������ִ�н��
	@type	return���Ӿ����Դ�������
	--]]
	local status, ret = pcall(loadstring(strCode))
	assert(status, ret)
	return ret
end
--------------------------------------------------------------------------------
function smartImport(name, path, autoRelease)
	--[[
	����ģ�飬��������Ӧ��ֵ
	@param	name��ģ������
	@type	name��string or nil
	@param	path: ģ���ļ����·��
	@type	path��string or nil
	@param	autoRelease: �Ƿ��Զ��ͷ��ڲ�����
	@type	autoRelease: boolean or nil
	@param	return: module{class or function or table or nil}
	@type	return: ��module����
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
	������arg���
	@param	arg����arg��ɵ�arg
	@type	arg��(arg1��arg2�� ..., count)
	@param	return: arg�����Ĳ����б�
	@type	return�������б�
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
	��ȫ��table(_G)�в���object������
	@param	object: ��һ�����luaֵ
	@type	object: any
	@param	return: object������
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
	��ȡobject�ļ�ֵ��
	��1��objectΪtableʱ������table���ڴ��ַ
	��2��Ϊnilʱ������""
	��3�������������tostring(object)
	@param	return: object��keyֵ
	@type	return: string
	--]]
	local _, _, key = string.find(tostring(object), ":%s*(%w+)")
	return key or (object or tostring(object) and "")
end
--------------------------------------------------------------------------------
function getObjectsKey(...)
	--[[
	��ȡ���������keyֵ��keyֵ����"_"����
	@param	arg: �������
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
	��valueתΪ��������; ת��ʧ��ʱ,ʹ��defaultֵ
	@param	value: ����ֵ
	@type	value: any
	@param	default: Ĭ��ֵ
	@type	default��number
	@param	return: ת�����
	@type	return: number
	--]]
	local ret = tonumber(value)
	return ret and ret or (default or 0)
end
--------------------------------------------------------------------------------
function toBool(value)
	--[[
	��valueתΪbool����;����true�����:valueֵΪtrue, "true", "yes", "y"�� ����0��number
	@param	return: ת�����
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
	�ж��ַ���value��offsetƫ�������Ƿ���prefix�ַ�����ʼ
	@param	value: �����Ե��ַ���
	@type	value: string
	@param	prefix: ���Ե��ַ���
	@type	prefix:	string
	@param	offset: �ӱ������ַ�������߿�ʼ��ƫ����,��ߵ�һ��ƫ����Ϊ1
	@type	offset: number
	@param	return: ���Խ��
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
	�ж��ַ���value�Ƿ���suffix�ַ�������
	@param	value: �����Ե��ַ���
	@type	value: string
	@param	suffix: �����ַ���
	@type	suffix: string
	@param	return: ���Խ��
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
	���ַ���value��ߵ�һ����ĸ��Ϊ��д��ͨ����property����prototype�����ú�����ʱ�õ�
	@param	value: 
	@type	value��string
	@param	return: value�ı���
	@type	return: string
	--]]
	return string.upper(string.sub(value, 1, 1)) .. string.sub(value, 2, #value)
end
--------------------------------------------------------------------------------
function string.ltitle(value)
	--[[
	���ַ���value��ߵ�һ����ĸ��ΪСд
	@param	value: 
	@type	value��string
	@param	return: value�ı���
	@type	return: string
	--]]
	return string.lower(string.sub(value, 1, 1)) .. string.sub(value, 2, #value)
end
--------------------------------------------------------------------------------
function string.split(s, seg)
	--[[
	���ַ���s���շָ���Ϊseg,�ָ�浽һ��table
	@param	s�����ָ��Դ�ַ���
	@type	s��string
	@param	seg���ָ��ַ���
	@type	seg��string
	@param	return���ָ���
	@type	return��table array{string}
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
	��ȡ�ַ���posλ�ô����ַ���posΪ��ʱ�������ң�1��ʼ������Ϊ��ʱ�����ҵ���-1��ʼ�ݼ�
	@param	value��
	@type	value��string
	@param	pos��
	@type	pos��number
	@param	return���ַ���posλ�ô����ַ���pos����value��Χʱ����nil
	@type	return��string
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
	�ж�value�Ƿ�ȫ���ɿո�(" ")�����Ʊ��("\t")���
	@param	value:
	@type	value: string
	@param	return: �жϽ��
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
	���ɱ�����ַ�����
	@param	arg��
	@type	arg��
	@param	return��
	@type	return��string
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
	���ַ���value�����ַ���,������½��ı�����
	@param	value: 
	@type	value: string
	@param	return: ����ַ���value�ı�
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
	���ַ���value�л�ȡposλ�ÿ�ʼ������Ϊlength�����ַ���
	@param	value��ԭ�ַ���
	@type	value��string
	@param	pos�������ң���1��ʼ��Ҫ������ַ�����λ��
	@type	pos��number
	@param	length��Ҫ������ַ����ĳ���
	@type	length��number
	@param	return�����ַ���
	@type	return��string
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
	��Դ��(source)ǳ��copy��Ŀ���(destiny);���ҽ���overlayΪfalseʱ,������destinyԭ�е�ֵ
	@param	source: Ҫ������Դ��
	@type	source: table
	@param	destiny: ��Ž���ı�,����ֵΪnilʱ,���½�һ����
	@type	destiny: table or nil
	@param	overlay: ��־�Ƿ񸲸�Ŀ���ԭ�е�ֵ(Ĭ�ϸ��ǣ�
	@type	overlay: boolean(true or false)
	@param	return: ����ǳ�ȿ����ı�;��destinyΪnilʱ,���ش�Ž�����±�,���򷵻�destiny��
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
	���copy
	@param	source: Ҫ������Դ��
	@type	source: table
	@param	destiny: ��Ž���ı�,����ֵΪnilʱ,���½�һ����
	@type	destiny: table or nil
	@param	replace: �Ƿ��滻ԭ����ֵ; Ϊfalse����nilʱ��ʹ��table.insert,Դkey���ٱ���,copy����
	@type	replace: boolean or nil
	@param	return: ������ȿ����ı�;��destinyΪnilʱ,���ش�Ž�����±�,���򷵻�destiny��
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
	��һ��table���Ƶ���һ��, ���ᱣ��keyֵ����Ŀ��table�����ܵ�Ӱ��
	@param	source: Ҫ������Դ��
	@type	source: table
	@param	destiny: ��Ž���ı�,����ֵΪnilʱ,���½�һ����
	@type	destiny: table or nil
	@param	return: ������ȿ����ı�;��destinyΪnilʱ,���ش�Ž�����±�,���򷵻�destiny��
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
	����table��ɵĲ����б�(arg)���ӵ��½��ı�,����Դ���keyֵ
	@param arg: ��table��ɵĲ����б�
	@type arg: {table, table, ...}
	@param	return: �����б�(arg)���ӳɵ��±�
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
	����tbl���
	@param	tbl������յı�
	@type	tbl��table
	@param	return����պ��tbl��
	@type	return��table
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
	�������tbl����ȥvalue��ֵ
	@param	tbl������Դ��
	@type	tbl��table
	@param	value����Ҫ��ȥ��ֵ
	@type	value��any
	@param	return����ȥvalueֵ���tbl
	@type	return��table
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
	��Map��tbl����ȥvalue��ֵ
	@param	tbl��MapԴ��
	@type	tbl��table
	@param	value����Ҫ��ȥ��ֵ
	@type	value��any
	@param	return����ȥvalueֵ���tbl
	@type	return��table
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
	�ڱ�tbl����ȥvalue��ֵ
	@param	tbl��Դ��
	@type	tbl��table
	@param	value����Ҫ��ȥ��ֵ
	@type	value��any
	@param	return����ȥvalueֵ���tbl
	@type	return��table
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
	�ӱ�tbl����ȥ��ֵΪkey�ļ���ֵ
	@param	tbl��Դ��
	@type	tbl��table
	@param	key����Ҫ��ȥ�ļ�ֵ
	@type	key��string or number
	@param	return����ȥ��ֵkey���tbl
	@type	return��table
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
	���Ա�tbl�Ƿ����ֵvalue
	@param	tbl: Դ��
	@type	tbl: table
	@param	key��Ҫ���Ե�ֵ
	@type	key��any
	@param	return�����Խ��
	@type	return��boolean(true or false)
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
	���Ա�tbl�Ƿ���ڼ�key
	@param	tbl:Դ��
	@type	tbl:table
	@param	key:Ҫ���Եļ�
	@type	key: number or string
	@param	return: ���Խ��
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
	���tbl�ĳ���;�൱������ȵĲ�����#
	@param	tbl: Դ��
	@type	tbl: table
	@param	return: ����
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
	��ȡ���ʱ�tbl�ĵ�������
	@param	tbl: Դ��
	@type	tbl: table
	@param	return: ����tbl�ĵ�������
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
	��ȡ���ʱ�tbl�������������
	@param	tbl: Դ��,array��ʽ
	@type	tbl: table
	@param	return: ����tbl�ĵ�������
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
	��ȡ��comparator������ķ���tbl�ĵ�������
	@param	tbl: Դ��
	@type	tbl��table
	@param	comparator: �ȽϺ���; ���ӣ������������ص�������ʱfunction(a, b) return a < b end
	@type	comparator: function(a, b)
	@param	return: ��comparator������ķ���tbl�ĵ�������
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
	����Դ���Ƿ����object
	@param	tbl�� Դ��
	@type	tbl��table
	@param	object������ֵ
	@type	object��any
	@param	return�����Խ��
	@type	return��boolean��true or false��
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
	���Ա�tbl�Ƿ����ĳ��Ԫ��element,֧������(����k,vͬʱ���)
	@param	tbl�� Դ��
	@type	tbl��table
	@param	element������Ԫ��
	@type	element��any
	@param	return�����Խ��
	@type	return��boolean��true or false��
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
	���Ա�tbl�Ƿ����һ��Ԫ��
	@param	tbl�� Դ��
	@type	tbl��table
	@param	elements������Ԫ����
	@type	elements��table
	@param	return�����Խ��
	@type	return��boolean��true or false��
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
	�жϱ�tbl�ļ�ֵkey�Ƿ�ȫ���������
	@param	tbl��Դ��
	@type	tbl��table
	@param	return�����Խ��
	@type	return��boolean��true or false��
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
	���Ա�tbl�Ƿ�Ϊmap����tbl��key��ȫ��number���
	@param	tbl��Դ��
	@type	tbl��table
	@param	return�����Խ��
	@type	return��boolean��true or false��
	--]]
	if not tbl then
		return false
	end
	return table.isArray(tbl) ~= true
end
--------------------------------------------------------------------------------
function table.unpack(tbl)
	--[[
	��tbl���Ϊ�����б�
	@param	tbl��Դ��
	@type	tbl��table
	@param	return�������Ĳ����б�
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
	���ɱ�������Ϊһ��table(����arg������Lua5.1�±�׼)
	@param	arg: �ɱ�����б�
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
	�ڱ�tbl�л�ȡֵΪvalue�ļ�
	@param	tbl: Դ��
	@type	tbl: table
	@param	value: Ҫ���Ե�ֵ
	@type	value: any
	@param	return: ��
	@type	return��string or int or nil
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
	�ڱ�tbl�л�ȡֵΪvalue�ļ���
	@param	tbl: Դ��
	@type	tbl: table
	@param	value: Ҫ���Ե�ֵ
	@type	value: any
	@param	return: ��ֵ
	@type	return��string
	--]]
	return tostring(table.getKey(tbl, value) or "")
end
--------------------------------------------------------------------------------
function table.save(tbl,fname,name)
	--[[
	����tbl��name�����ֱ��浽�ļ�fname��
	@param	tbl: Դ��
	@type	tbl��table
	@param	fname���ļ���
	@type	fname��string
	@param	name������
	@type	name��string
	@param	return�����ر�����������ɹ�ʱ����true��ʧ�ܷ���false
	@type	return��boolean(true or false)
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
	����n1~n2��������
	@param	n1:
	@type	n1: number
	@param	n2:
	@param	n2: number
1	@param	return: n1~n2��������
	@type	return: number
	--]]
	return math.random(n1*10000, n2*10000)/10000
end
--------------------------------------------------------------------------------
function math.round(n)
	--[[
	eg.
	��1��math.round(-4.5) �� -4
	��2��math.round(4.5)��	5
	@param	n��
	@type	n��number
	@param	return:
	@type	return��int
	--]]
	return math.floor(0.5 + n)
end
