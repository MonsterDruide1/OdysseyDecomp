#include "Project/Bgm/BgmInfo.h"

namespace al {

int BgmUserInfo::compareInfo(const BgmUserInfo* info_1, const BgmUserInfo* info_2){
  return strcmp(info_1->mName,info_2->mName);
}
int al::BgmUserInfo::compareInfoByKey(const BgmUserInfo* param_1,const char* param_2){
  return strcmp(param_1->mName,param_2);
}


}