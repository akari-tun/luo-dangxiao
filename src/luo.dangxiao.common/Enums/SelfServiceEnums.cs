using System;
using System.Collections.Generic;
using System.Text;

namespace luo.dangxiao.common.Enums
{
    /// <summary>
    /// Self service type enumeration
    /// </summary>
    public enum SelfServiceType
    {
        StaffSelfService,
        StudentSelfService
    }

    /// <summary>
    /// Home page state enumeration
    /// </summary>
    public enum HomePageState
    {
        HomePage,
        SubPageContainer
    }

    /// <summary>
    /// User type enumeration
    /// </summary>
    public enum UserType
    {
        Staff,
        Student
    }

    /// <summary>
    /// Verification method for verify page.
    /// </summary>
    public enum VerifyMethod
    {
        IDCard,
        SMS
    }

}
