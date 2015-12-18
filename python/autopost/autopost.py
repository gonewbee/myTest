import requests
from lxml import etree
import config

def myLoginTest():
    login = 'http://'+config.hostname+'/login.aspx'
    headers = {'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/43.0.2357.81 Safari/537.36'}
    print(login)
    r = requests.get(login, headers=headers)
    print(r.encoding, r.status_code)
    # with open('out.html', 'w') as fo:
    #     fo.write(r.text.encode('utf-8'))
    page = etree.HTML(r.text)
    LastToken = page.xpath("//input[@id='__LastToken']")
    ZIPSTATE = page.xpath("//input[@id='__ZIPSTATE']")
    print(LastToken[0].attrib['value'])
    print(ZIPSTATE[0].attrib['value'])
    payload = {'__LastToken':LastToken[0].attrib['value'],
               '__ZIPSTATE':ZIPSTATE[0].attrib['value'],
               '__VIEWSTATE':'',
               'btnLogin':'',
               'txtUser':config.user,
               'txtPassword':config.passwd}
    rlogin = requests.post(login, headers=headers, cookies=r.cookies, data=payload)
    print(rlogin.cookies)
    print('myLoginTest end!!!')

def myLogin():
    '''
    使用requests.Session()进行请求，不用再自己处理cookie数据
    '''
    login = 'http://'+config.hostname+'/login.aspx'
    headers = {'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/43.0.2357.81 Safari/537.36'}
    print(login)
    session = requests.Session()
    resp = session.get(login, headers=headers)
    print(resp.encoding, resp.status_code)
    page = etree.HTML(resp.text)
    LastToken = page.xpath("//input[@id='__LastToken']")[0].attrib['value']
    ZIPSTATE = page.xpath("//input[@id='__ZIPSTATE']")[0].attrib['value']
    print('LastToken:%s ZIPSTATE:%s' % (LastToken, ZIPSTATE))
    payload = {'__LastToken':LastToken,
               '__ZIPSTATE':ZIPSTATE,
               '__VIEWSTATE':'',
               'btnLogin':'',
               'txtUser':config.user,
               'txtPassword':config.passwd}
    resp = session.post(login, headers=headers, data=payload)
    print(session.cookies)
    daylylist = 'http://'+config.hostname+'/daylylist.aspx'
    resp = session.get(daylylist, headers=headers)
    with open('daylyout.html', 'w') as fo:
        fo.write(resp.text.encode('utf-8'))
    print('myLogin end')

if __name__=='__main__':
    # myLoginTest()
    myLogin()