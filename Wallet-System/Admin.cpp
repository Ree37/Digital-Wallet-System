//#include "Admin.h"
//#include "User.h"
//
//int Admin::get_Index(string user_name){
//    for(int i =0;i<=users.size();i++){
//        if(users[i].name==user_name){
//            return i;
//        }else{
//            continue;
//        }
//
//    }
//}
//void Admin::delete_User(string deleted_User_Name ){
//
//    int position= Admin::get_Index(deleted_User_Name);
//
//   Admin::users.erase(position);
//   unique_Users_Name.erase(deleted_User_Name);
//}
//void Admin::suspend_User(string suspended_User_Name ){
//  int position=  int position= Admin::get_Index(suspended_User_Name);
//     users.at(position).susbended_Flag=true;
//}
//void Admin::adjust_User_Balance(string adjusted_user_name,int value){
//    int position=  int position= Admin::get_Index(suspended_User_Name);
//
//    users.at(position).balance=value
//
//}
//void Admin::view_Users()
//{
//    for (int i = 0; i <= users.size(); i++) {
//        cout << "Username : " << users.at(i).name << "\t Balance : " << users.at(i).balance
//             << endl;
//        for (int j = 0; j < users.at(i).transactions.size();i++)
//        {
//            cout << "transaction"<<j+1<<users.at(i).transactions.at(j);
//        }
//        cout<<"***************************************";
//
//}
//bool Admin::is_Unique(string name){
//
//    return !unique_Users_Name.count(name);
//
//}
