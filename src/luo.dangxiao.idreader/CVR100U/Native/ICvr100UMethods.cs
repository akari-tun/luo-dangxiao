using System.Text;

namespace luo.dangxiao.idreader.CVR100U.Native;

/// <summary>
/// Interface for CVR100U SDK native methods.
/// Provides a unified contract for platform-specific P/Invoke implementations.
/// </summary>
public interface ICvr100UMethods
{
    /// <summary>
    /// Initializes communication via USB OTG (Linux specific).
    /// </summary>
    /// <param name="path">Device path (null for auto-detect).</param>
    /// <param name="protocolType">Protocol type (e.g., 2 for USB-OTG).</param>
    /// <returns>1 on success.</returns>
    int CVR_InitComm(string? path, int protocolType);

    /// <summary>
    /// Initializes communication via USB port (Windows specific).
    /// </summary>
    /// <param name="port">USB port number (e.g., 1001).</param>
    /// <returns>1 on success.</returns>
    int CVR_InitComm(int port);

    /// <summary>
    /// Closes the communication.
    /// </summary>
    /// <returns>1 on success.</returns>
    int CVR_CloseComm();

    /// <summary>
    /// Authenticates the ID card in the reader.
    /// Uses CVR_AuthenticateForNoJudge on Linux and CVR_Authenticate on Windows.
    /// </summary>
    /// <returns>1 on success.</returns>
    int CVR_Authenticate();

    /// <summary>
    /// Reads the card content.
    /// </summary>
    /// <param name="active">Active mode flag (1 for active read).</param>
    /// <returns>1 on success.</returns>
    int CVR_Read_Content(int active);

    /// <summary>
    /// Gets the security module (SAM) ID.
    /// </summary>
    /// <param name="buffer">Buffer to receive the SAM ID.</param>
    /// <param name="length">Buffer size (in) and actual length (out).</param>
    /// <returns>1 on success.</returns>
    int CVR_GetSAMID(StringBuilder buffer, ref int length);

    /// <summary>
    /// Gets the card UID.
    /// </summary>
    /// <param name="buffer">Buffer to receive the UID bytes.</param>
    /// <param name="length">Buffer size (in) and actual length (out).</param>
    /// <returns>1 on success.</returns>
    int CVR_GetUID(byte[] buffer, ref int length);

    /// <summary>
    /// Gets the card holder name.
    /// </summary>
    int GetPeopleName(StringBuilder buffer, ref int length);

    /// <summary>
    /// Gets the card holder gender.
    /// </summary>
    int GetPeopleSex(StringBuilder buffer, ref int length);

    /// <summary>
    /// Gets the card holder nation/ethnicity.
    /// </summary>
    int GetPeopleNation(StringBuilder buffer, ref int length);

    /// <summary>
    /// Gets the card holder birthday.
    /// </summary>
    int GetPeopleBirthday(StringBuilder buffer, ref int length);

    /// <summary>
    /// Gets the card holder ID number.
    /// </summary>
    int GetPeopleIDCode(StringBuilder buffer, ref int length);

    /// <summary>
    /// Gets the issuing department/authority.
    /// </summary>
    int GetDepartment(StringBuilder buffer, ref int length);

    /// <summary>
    /// Gets the ID card validity start date.
    /// </summary>
    int GetStartDate(StringBuilder buffer, ref int length);

    /// <summary>
    /// Gets the ID card validity end date.
    /// </summary>
    int GetEndDate(StringBuilder buffer, ref int length);

    /// <summary>
    /// Gets the card holder address.
    /// </summary>
    int GetPeopleAddress(StringBuilder buffer, ref int length);

    /// <summary>
    /// Gets the card holder photo data (BMP format).
    /// </summary>
    int GetBMPData(byte[] buffer, ref int length);

    /// <summary>
    /// Gets the card type identifier.
    /// </summary>
    int GetCertType(byte[] buffer, ref int length);
}
