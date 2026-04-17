# 鑷姪鏈嶅姟椤甸潰璁捐鏂囨。鐩綍

> **鏂囨。璇存槑**: 鏈洰褰曞寘鍚嚜鍔╂湇鍔＄郴缁熸墍鏈夐〉闈㈢殑璇︾粏璁捐鏂囨。  
> **绯荤粺**: 鑷姪鏈嶅姟缁堢 (SelfService)  
> **鐢ㄦ埛**: 瀛﹀憳銆佹暀鑱屽伐  
> **鏈€鍚庢洿鏂?*: 2026-03-26

---

## 涓€銆侀〉闈㈠垎绫?
绯荤粺椤甸潰鍒嗕负涓夌被锛?
### 1.1 涓婚〉闈?(1涓?

| 鏂囨。 | 椤甸潰浠ｇ爜 | 璇存槑 |
|------|----------|------|
| [HomePage](./HomePage.md) | HomePage | 涓荤晫闈㈠鍣紝涓夌鐘舵€侊細鏁欒亴宸ヨ嚜鍔?瀛﹀憳鑷姪/瀛愰〉闈㈠鍣?|

### 1.2 鍔熻兘瀛愰〉闈?(10涓?

| 鏂囨。 | 椤甸潰浠ｇ爜 | 璇存槑 | 鐢ㄦ埛绫诲瀷 |
|------|----------|------|----------|
| VerifyPage.md | VerifyPage | 缁熶竴楠岃瘉鍏ュ彛 | 鍏ㄩ儴 |
| TakeCardPage.md | TakeCardPage | 鑷姪鍙栧崱 | 鍏ㄩ儴 |
| ReportLossPage.md | ReportLossPage | 鑷姪鎸傚け | 鍏ㄩ儴 |
| ReplacementPage.md | ReplacementPage | 鑷姪琛ュ崱 | 鍏ㄩ儴 |
| QueryPage.md | QueryPage | 鑷姪鏌ヨ | 鏁欒亴宸?|
| RechargePage.md | RechargePage | 鑷姪鍏呭€?| 鏁欒亴宸?|
| CheckInPage.md | CheckInPage | 鑷姪鎶ュ埌 | 瀛﹀憳 |
| CardProcessPage.md | CardProcessPage | 鍗＄墖澶勭悊杩囩▼ | 鍏ㄩ儴 |
| CompletionPage.md | CompletionPage | 鎿嶄綔瀹屾垚 | 鍏ㄩ儴 |

### 1.3 妯″潡椤甸潰 (4涓?

| 鏂囨。 | 椤甸潰浠ｇ爜 | 璇存槑 | 鐢ㄩ€?|
|------|----------|------|------|
| StudentInfoPage.md | StudentInfoPage | 瀛﹀憳淇℃伅妯″潡 | 宓屽叆鍔熻兘瀛愰〉闈㈠睍绀哄鍛樹俊鎭?|
| StaffInfoPage.md | StaffInfoPage | 鏁欒亴宸ヤ俊鎭ā鍧?| 宓屽叆鍔熻兘瀛愰〉闈㈠睍绀烘暀鑱屽伐淇℃伅 |
| IDCardVerifyPage.md | IDCardVerifyPage | 韬唤璇侀獙璇佹ā鍧?| 宓屽叆VerifyPage |
| SMSVerifyPage.md | SMSVerifyPage | 鐭俊楠岃瘉妯″潡 | 宓屽叆VerifyPage |

---

## 浜屻€侀〉闈㈡灦鏋?
### 2.1 HomePage 浣滀负瀹瑰櫒

```
鈹屸攢鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹?鈹? HEADER (椤堕儴鏍?                    鈹?鈹? [Logo] 鏍囬 [鍊掕鏃禲               鈹?鈹溾攢鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹?鈹?                                    鈹?鈹?    鐘舵€?: 鏁欒亴宸?瀛﹀憳鑷姪          鈹?鈹?    鈹屸攢鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹?      鈹?鈹?    鈹?鍔熻兘鎸夐挳鑿滃崟          鈹?      鈹?鈹?    鈹?[鍙栧崱][鎸傚け][琛ュ崱]... 鈹?      鈹?鈹?    鈹斺攢鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹?      鈹?鈹?                                    鈹?鈹?    鐘舵€?: 瀛愰〉闈㈠鍣?              鈹?鈹?    鈹屸攢鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹?      鈹?鈹?    鈹?                      鈹?      鈹?鈹?    鈹?  鍔熻兘瀛愰〉闈㈠唴瀹?      鈹?      鈹?鈹?    鈹?  (宓屽叆鏄剧ず)          鈹?      鈹?鈹?    鈹?                      鈹?      鈹?鈹?    鈹斺攢鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹?      鈹?鈹?                                    鈹?鈹溾攢鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹?鈹? [杩斿洖鎸夐挳]  [鎿嶄綔鎸夐挳]             鈹?鈹斺攢鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹?```

### 2.2 椤甸潰鍔犺浇鍏崇郴

```
HomePage (瀹瑰櫒)
    鈹溾攢鈹€ 鍔犺浇 VerifyPage
    鈹?      鈹溾攢鈹€ 宓屽叆 IDCardVerifyPage (韬唤璇侀獙璇?
    鈹?      鈹斺攢鈹€ 宓屽叆 SMSVerifyPage (鐭俊楠岃瘉)
    鈹?    鈹溾攢鈹€ 鍔犺浇 TakeCardPage
    鈹?      鈹溾攢鈹€ 宓屽叆 StaffInfoPage (鏁欒亴宸?
    鈹?      鈹斺攢鈹€ 宓屽叆 StudentInfoPage (瀛﹀憳)
    鈹?    鈹溾攢鈹€ 鍔犺浇 ReportLossPage
    鈹?      鈹溾攢鈹€ 宓屽叆 StaffInfoPage (鏁欒亴宸?
    鈹?      鈹斺攢鈹€ 宓屽叆 StudentInfoPage (瀛﹀憳)
    鈹?    鈹溾攢鈹€ 鍔犺浇 ReplacementPage
    鈹?      鈹溾攢鈹€ 宓屽叆 StaffInfoPage (鏁欒亴宸?
    鈹?      鈹斺攢鈹€ 宓屽叆 StudentInfoPage (瀛﹀憳)
    鈹?    鈹溾攢鈹€ 鍔犺浇 QueryPage
    鈹?      鈹斺攢鈹€ 宓屽叆 StaffInfoPage (鏁欒亴宸?
    鈹?    鈹溾攢鈹€ 鍔犺浇 RechargePage
    鈹?      鈹溾攢鈹€ 宓屽叆 StaffInfoPage (鏁欒亴宸?
    鈹?      鈹斺攢鈹€ 鍔犺浇 QRCodePage (鏀粯)
    鈹?    鈹溾攢鈹€ 鍔犺浇 CheckInPage
    鈹?      鈹斺攢鈹€ 宓屽叆 StudentInfoPage (瀛﹀憳)
    鈹?    鈹溾攢鈹€ 鍔犺浇 CardProcessPage
    鈹?    鈹斺攢鈹€ 鍔犺浇 CompletionPage
```

---

## 涓夈€佸姛鑳芥祦绋?
### 3.1 鏁欒亴宸ヨ嚜鍔╂祦绋?
| 鍔熻兘 | 椤甸潰娴佺▼ |
|------|----------|
| **鑷姪鍙栧崱** | HomePage 鈫?VerifyPage 鈫?TakeCardPage 鈫?CardProcessPage 鈫?CompletionPage |
| **鑷姪鎸傚け** | HomePage 鈫?VerifyPage 鈫?ReportLossPage 鈫?CardProcessPage 鈫?CompletionPage |
| **鑷姪琛ュ崱** | HomePage 鈫?VerifyPage 鈫?ReplacementPage 鈫?CardProcessPage 鈫?CompletionPage |
| **鑷姪鏌ヨ** | HomePage 鈫?VerifyPage 鈫?QueryPage |
| **鑷姪鍏呭€?* | HomePage 鈫?VerifyPage 鈫?RechargePage 鈫?QRCodePage 鈫?CardProcessPage 鈫?CompletionPage |

### 3.2 瀛﹀憳鑷姪娴佺▼

| 鍔熻兘 | 椤甸潰娴佺▼ |
|------|----------|
| **鑷姪鍙栧崱** | HomePage 鈫?VerifyPage 鈫?TakeCardPage 鈫?CardProcessPage 鈫?CompletionPage |
| **鑷姪鎸傚け** | HomePage 鈫?VerifyPage 鈫?ReportLossPage 鈫?CardProcessPage 鈫?CompletionPage |
| **鑷姪琛ュ崱** | HomePage 鈫?VerifyPage 鈫?ReplacementPage 鈫?CardProcessPage 鈫?CompletionPage |
| **鑷姪鎶ュ埌** | HomePage 鈫?VerifyPage 鈫?CheckInPage 鈫?CompletionPage |

---

## 鍥涖€佹枃妗ｉ槄璇绘寚鍗?
### 4.1 鏂囨。缁撴瀯

姣忎釜椤甸潰璁捐鏂囨。鍖呭惈锛?1. **璁捐鍥惧紩鐢?* - 璁捐鍥炬簮鏂囦欢銆佸垏鍥捐祫婧?2. **椤甸潰甯冨眬缁撴瀯** - ASCII 甯冨眬鍥俱€佺姸鎬佸睍绀?3. **UI 鍏冪礌璇﹁В** - 椤堕儴鏍忋€佸唴瀹瑰尯銆佸簳閮ㄦ寜閽?4. **鍔熻兘鎻忚堪** - 椤甸潰鐢ㄩ€斻€佹牳蹇冨姛鑳?5. **浜や簰鎻忚堪** - 鐘舵€佽浆鎹€佷氦浜掓祦绋?6. **璁捐瑙勮寖** - 棰滆壊銆佸瓧浣撱€侀棿璺?7. **瀹炵幇娉ㄦ剰浜嬮」** - 鏁版嵁妯″瀷銆乂iewModel
8. **鐩稿叧鏂囨。** - 鍏宠仈鏂囨。閾炬帴

### 4.2 閫氱敤瑙勮寖

| 瑙勮寖椤?| 鍊?|
|--------|-----|
| 涓昏壊璋?| 绾㈣壊绯?#C42828, #d9230b |
| 杈呭姪鑹?| 閲戦粍鑹?#e6b84d, #ffe3a3 |
| 鍐呭鍖鸿儗鏅?| 绫抽粍鑹?#faf9f3 |
| 涓枃瀛椾綋 | 榛戜綋 (SimHei) |
| 鑻辨枃瀛椾綋 | Arial |
| 鍗＄墖鍦嗚 | 7px |
| 鎸夐挳鍦嗚 | 8px |
| 鍊掕鏃朵綅缃?| 宸︿笂瑙掞紝#ff6000 |

---

## 浜斻€佸揩閫熷弬鑰?
### 5.1 椤甸潰绫诲瀷閫熸煡

| 椤甸潰 | 绫诲瀷 | 瀹瑰櫒 | 宓屽叆妯″潡 |
|------|------|------|----------|
| HomePage | 涓婚〉闈?| - | - |
| VerifyPage | 鍔熻兘瀛愰〉闈?| HomePage | IDCardVerifyPage, SMSVerifyPage |
| TakeCardPage | 鍔熻兘瀛愰〉闈?| HomePage | StaffInfoPage/StudentInfoPage |
| ReportLossPage | 鍔熻兘瀛愰〉闈?| HomePage | StaffInfoPage/StudentInfoPage |
| ReplacementPage | 鍔熻兘瀛愰〉闈?| HomePage | StaffInfoPage/StudentInfoPage |
| QueryPage | 鍔熻兘瀛愰〉闈?| HomePage | StaffInfoPage |
| RechargePage | 鍔熻兘瀛愰〉闈?| HomePage | StaffInfoPage |
| CheckInPage | 鍔熻兘瀛愰〉闈?| HomePage | StudentInfoPage |
| CardProcessPage | 鍔熻兘瀛愰〉闈?| HomePage | - |
| CompletionPage | 鍔熻兘瀛愰〉闈?| HomePage | - |
| StudentInfoPage | 妯″潡椤甸潰 | 宓屽叆鍔熻兘瀛愰〉闈?| - |
| StaffInfoPage | 妯″潡椤甸潰 | 宓屽叆鍔熻兘瀛愰〉闈?| - |
| IDCardVerifyPage | 妯″潡椤甸潰 | 宓屽叆VerifyPage | - |
| SMSVerifyPage | 妯″潡椤甸潰 | 宓屽叆VerifyPage | - |

### 5.2 鐢ㄦ埛绫诲瀷鍙敤椤甸潰

**瀛﹀憳鍙敤**:
- HomePage (瀛﹀憳鑷姪鐘舵€?
- VerifyPage 鈫?TakeCardPage, ReportLossPage, ReplacementPage, CheckInPage
- CardProcessPage, CompletionPage
- StudentInfoPage (妯″潡)

**鏁欒亴宸ュ彲鐢?*:
- HomePage (鏁欒亴宸ヨ嚜鍔╃姸鎬?
- VerifyPage 鈫?TakeCardPage, ReportLossPage, ReplacementPage, QueryPage, RechargePage
- CardProcessPage, CompletionPage
- StaffInfoPage (妯″潡)

---

## 鍏€佺浉鍏虫枃妗?
- [鑷姪鏈嶅姟璁捐瑙勬牸璇存槑涔(../璁捐瑙勬牸璇存槑涔?md)
- [鍗″姟涓績璁捐](../../CardCenter/)

---

## 涓冦€佺増鏈巻鍙?
| 鐗堟湰 | 鏃ユ湡 | 鏇存柊鍐呭 | 浣滆€?|
|------|------|----------|------|
| 1.0 | 2026-03-26 | 鍒涘缓椤甸潰璁捐鏂囨。鐩綍 | OpenCode Agent |
| 2.0 | 2026-03-26 | 鏇存柊椤甸潰鍒嗙被锛?br>鈥?鏄庣‘涓夌被椤甸潰锛堜富椤甸潰/鍔熻兘瀛愰〉闈?妯″潡椤甸潰锛?br>鈥?鎻忚堪HomePage瀹瑰櫒鏈哄埗<br>鈥?瀹屽杽鍔熻兘娴佺▼<br>鈥?娣诲姞椤甸潰鍏崇郴鍥?| OpenCode Agent |

---

*鏂囨。缁撴潫*


