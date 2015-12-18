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
