public boolean dbInsertKYC(KYC kyc) throws SQLException {
    String sql = "INSERT INTO kyc (account_id, full_name, national_id, address, phone, email) VALUES (?, ?, ?, ?, ?, ?);";
    PreparedStatement pstmt = conn.prepareStatement(sql);
    pstmt.setInt(1, kyc.getAccountId());
    pstmt.setString(2, kyc.getFullName());
    pstmt.setString(3, kyc.getNationalId());
    pstmt.setString(4, kyc.getAddress());
    pstmt.setString(5, kyc.getPhone());
    pstmt.setString(6, kyc.getEmail());
    int result = pstmt.executeUpdate();
    pstmt.close();
    return result > 0;
}

public KYC dbGetKYC(int accountId) throws SQLException {
    String sql = "SELECT * FROM kyc WHERE account_id = ?;";
    PreparedStatement pstmt = conn.prepareStatement(sql);
    pstmt.setInt(1, accountId);
    ResultSet rs = pstmt.executeQuery();
    KYC kyc = null;
    if (rs.next()) {
        kyc = new KYC(
            rs.getInt("account_id"),
            rs.getString("full_name"),
            rs.getString("national_id"),
            rs.getString("address"),
            rs.getString("phone"),
            rs.getString("email")
        );
    }
    rs.close();
    pstmt.close();
    return kyc;
}

