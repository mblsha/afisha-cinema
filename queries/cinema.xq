<cinema detailed="true">
<name>{doc($input)//b[@class = 'title_red']/text()}</name>
<id>{fn:replace(doc($input)//div[@class = 'content']/div[@class = 'small_text']/a/@href, '.*(?:&amp;|\?)place_id=(\d+).*', '$1')}</id>
<address>{fn:replace(doc($input)//div[@class = 'pad']/div[@class = 'content']/a/text(), '\n', '')}</address>
<metro>{fn:replace(fn:string-join(doc($input)//div[@class = 'pad']/div[@class = 'content']/text(), '\n'), '.*(?:\\n\s*м.\s+([\w\s-\.]+)\\n)*.*', '$1')}</metro>
{
for $f in fn:tokenize(fn:replace(fn:string-join(doc($input)//div[@class = 'pad']/div[@class = 'content']/text(), '\n'), '.*\\n\s*тел.:\s*((?:(?:(?:\(?\d{3}\)\s*)*\d{3}\s*\d{2}\s*\d{2})(?:,\s*)*)+).*', '$1'), ',\s*')
return
<phone>{$f}</phone>
}
<details>{doc($input)//div[@class = 'content']/div/text()}</details>
</cinema>
