/*=================================================== 
* 命名空间：com.zd.business.apiservice 
* 文 件 名：CardService 
* 内容简述：卡片相关Api接口服务
* 创 建 人：luoyibo
* 创建日期：2019-05-22 9:42:16 
* 版    本：V1.0 
* 版权: Copyright (C) 2018-2024 
*       深圳市宇川智能系统有限公司 All Rights Reserved
* 修改记录： 
* 日期        版本      修改人    修改内容    
=====================================================*/
using System.Text;
using com.zd.business.model.card;
using com.zd.framework.common.abstractmodel;
using com.zd.framework.common.apiservice;

namespace com.zd.business.apiservice {
    public class CardService: BaseService<CardService> {
        /// <summary>
        /// 根据卡流水号和物理卡号获取指定的卡片信息
        /// </summary>
        /// <param name="cardNo">卡流水号</param>
        /// <param name="cardFixNo">物理卡号</param>
        /// <param name="card">卡片信息</param>
        /// <returns></returns>
        public MethodResult getVoCard(string cardNo,string cardFixNo,out VoCard card) {
            StringBuilder sbPostContent = new StringBuilder("{\"factoryFixId\":\"");
            sbPostContent.Append(cardFixNo);
            sbPostContent.Append("\",\"cardNo\":\"");
            sbPostContent.Append(cardNo);
            sbPostContent.Append("\"}");

            card =  getOne<VoCard>("/card/api/v1/vocard", "请求卡片信息",sbPostContent.ToString(),"post");
            return new MethodResult(isSuccess, message);
        }

        /// <summary>
        /// 初始化一张新卡
        /// </summary>
        /// <param name="newCard"></param>
        /// <param name="operatorId"></param>
        /// <param name="workStationNumb"></param>
        /// <param name="getCard"></param>
        /// <returns></returns>
        public MethodResult initNewCard(VoCard newCard, string oldCardId,string cardOperate, string operatorId, string workStationNumb, out VoCard initedCard) {
            StringBuilder sbPostContent = new StringBuilder("{\"factoryFixId\":\"");
            sbPostContent.Append(newCard.factoryFixId);
            sbPostContent.Append("\",\"userId\":\"");
            sbPostContent.Append(newCard.userId);
            sbPostContent.Append("\",\"cardTypeId\":\"");
            sbPostContent.Append(newCard.cardTypeId);
            sbPostContent.Append("\",\"expiryDate\":\"");
            sbPostContent.Append(newCard.expiryDate.ToString("yyyy-MM-dd HH:mm:ss"));
            sbPostContent.Append("\",\"mainDeputyType\":\"");
            sbPostContent.Append(newCard.mainDeputyType);
            sbPostContent.Append("\",\"operatorId\":\"");
            sbPostContent.Append(operatorId);
            sbPostContent.Append("\",\"workStationNumb\":\"");
            sbPostContent.Append(workStationNumb);

            sbPostContent.Append("\",\"oldCardId\":\"");
            sbPostContent.Append(oldCardId);

            sbPostContent.Append("\",\"cardOperate\":\"");
            sbPostContent.Append(cardOperate);

            sbPostContent.Append("\",\"tenantId\":\"");
            sbPostContent.Append(newCard.tenantId);

            sbPostContent.Append("\"}");

            initedCard = getOne<VoCard>("/card/api/v1/init", "", sbPostContent.ToString(), "post");

            return new MethodResult(isSuccess, message);
        }

        /// <summary>
        /// 写卡成功
        /// </summary>
        /// <param name="newCard"></param>
        /// <param name="oldCardId"></param>
        /// <param name="cardOperate"></param>
        /// <param name="operatorId"></param>
        /// <param name="workStationNumb"></param>
        /// <returns></returns>
        public MethodResult writeNewCardSuccess(VoCard newCard, string oldCardId, string cardOperate, string operatorId, string workStationNumb) {
            StringBuilder sbPostContent = new StringBuilder("{\"factoryFixId\":\"");
            sbPostContent.Append(newCard.factoryFixId);
            sbPostContent.Append("\",\"cardId\":\"");
            sbPostContent.Append(newCard.cardId);
            sbPostContent.Append("\",\"cardNo\":\"");
            sbPostContent.Append(newCard.cardNo);
            sbPostContent.Append("\",\"userId\":\"");
            sbPostContent.Append(newCard.userId);
            sbPostContent.Append("\",\"cardTypeId\":\"");
            sbPostContent.Append(newCard.cardTypeId);

            sbPostContent.Append("\",\"operatorId\":\"");
            sbPostContent.Append(operatorId);
            sbPostContent.Append("\",\"workStationNumb\":\"");
            sbPostContent.Append(workStationNumb);

            sbPostContent.Append("\",\"oldCardId\":\"");
            sbPostContent.Append(oldCardId);

            sbPostContent.Append("\",\"cardOperate\":\"");
            sbPostContent.Append(cardOperate);

            sbPostContent.Append("\",\"tenantId\":\"");
            sbPostContent.Append(newCard.tenantId);

            sbPostContent.Append("\"}");

            string result = getString("/card/api/v1/write/success", "", sbPostContent.ToString(), "post");
            return new MethodResult(isSuccess, result);
        }
        /// <summary>
        /// 挂失用户卡片
        /// </summary>
        /// <param name="card"></param>
        /// <param name="cardOperate"></param>
        /// <param name="operatorId"></param>
        /// <param name="workStationNumb"></param>
        /// <returns></returns>
        public MethodResult lockUserCard(VoCard card, string cardOperate, string operatorId, string workStationNumb) {
            StringBuilder sbPostContent = new StringBuilder("{\"factoryFixId\":\"");
            sbPostContent.Append(card.factoryFixId);
            sbPostContent.Append("\",\"cardNo\":\"");
            sbPostContent.Append(card.cardNo);

            sbPostContent.Append("\",\"operatorId\":\"");
            sbPostContent.Append(operatorId);
            sbPostContent.Append("\",\"workStationNumb\":\"");
            sbPostContent.Append(workStationNumb);

            sbPostContent.Append("\",\"cardOperate\":\"");
            sbPostContent.Append(cardOperate);

            sbPostContent.Append("\",\"tenantId\":\"");
            sbPostContent.Append(card.tenantId);

            sbPostContent.Append("\"}");

            string result = getString("/card/api/v1/lock", "", sbPostContent.ToString(), "post");
            return new MethodResult(isSuccess, result);
        }

        /// <summary>
        /// 回收用户卡片
        /// </summary>
        /// <param name="card"></param>
        /// <param name="cardOperate"></param>
        /// <param name="operatorId"></param>
        /// <param name="workStationNumb"></param>
        /// <returns></returns>
        public MethodResult recycleUserCard(VoCard card, string cardOperate, string operatorId, string workStationNumb) {
            StringBuilder sbPostContent = new StringBuilder("{\"factoryFixId\":\"");
            sbPostContent.Append(card.factoryFixId);
            sbPostContent.Append("\",\"cardNo\":\"");
            sbPostContent.Append(card.cardNo);

            sbPostContent.Append("\",\"operatorId\":\"");
            sbPostContent.Append(operatorId);
            sbPostContent.Append("\",\"workStationNumb\":\"");
            sbPostContent.Append(workStationNumb);

            sbPostContent.Append("\",\"cardOperate\":\"");
            sbPostContent.Append(cardOperate);

            sbPostContent.Append("\"}");

            string result = getString("/card/api/v1/recycle", "", sbPostContent.ToString(), "post");
            return new MethodResult(isSuccess, result);
        }
    }
}
