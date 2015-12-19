#coding=utf-8
from lxml import etree

if __name__=='__main__':
    html = ''
    with open('out.html', 'r') as fi:
        html = fi.read()
    page = etree.HTML(html)
    LastToken = page.xpath("//input[@id='__LastToken']")
    ZIPSTATE = page.xpath("//input[@id='__ZIPSTATE']")
    print(LastToken[0].attrib['value'])
    print(ZIPSTATE[0].attrib['value'])
    # find获取第一个
    form = page.find(".//form[@name='form1']")
    inputs = form.xpath("//input")
    # 遍历form中的input
    for input in inputs:
        if 'name' in input.attrib:
            print(input.attrib['name'])
        if 'value' in input.attrib:
            print(input.attrib['value'].encode('utf-8'))

