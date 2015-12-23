#coding=utf-8
import requests
import time
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
    # proxies = {'http':'127.0.0.1:8888'}
    # 使用代理
    # resp = session.get(login, headers=headers, proxies=proxies)
    resp = session.get(login, headers=headers)
    print(resp.encoding, resp.status_code)
    page = etree.HTML(resp.text)
    form = page.find(".//form[@name='form1']")
    payload = {}
    inputs = form.xpath("//input")
    for input in inputs:
        if 'name' in input.attrib:
            if 'value' in input.attrib:
                payload[input.attrib['name']] = input.attrib['value']
            else:
                payload[input.attrib['name']] = ''
    payload['txtUser'] = config.user
    payload['txtPassword'] = config.passwd
    print(payload)
    resp = session.post(login, headers=headers, data=payload)
    print(session.cookies)
    return session

def getDayly(session):
    daylylist = 'http://'+config.hostname+'/daylylist.aspx'
    headers = {'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/43.0.2357.81 Safari/537.36'}
    resp = session.get(daylylist, headers=headers)
    # with open('daylyout.html', 'w') as fo:
    #     fo.write(resp.text.encode('utf-8'))
    # page = etree.HTML(resp.text)
    # jihuar = page.xpath("//input[@id='jihuanr']")[0].attrib['value']
    # wanchengzj = page.xpath("//input[@id='wanchengzj']")[0].attrib['value']
    # print(jihuar)
    # print(wanchengzj)
    print('getDayly end')

if __name__=='__main__':
    # myLoginTest()
    session = myLogin()
    getDayly(session)