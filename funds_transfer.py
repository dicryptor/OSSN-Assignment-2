import sys
import decimal

class XferLimitException(Exception):
    def __init__(self, exceedlimit):
        Exception.__init__(self, exceedlimit)


class myAccount:
    'Bank account class'
    xfer_limit = 5000
    TWODECS = decimal.Decimal(10) ** -2  # To check for two decimal places

    def __init__(self, acc_no, balance):
        self.acc_no = acc_no
        self.balance = balance
        print("Welcome account number {}-{}-{}".format(acc_no[:3],acc_no[3:6],acc_no[-3:]))
        print("Your account balance is {0:.2f}".format(balance))

    def checkamt(self, amt):
        try:
            self.amt = float(amt)
        except ValueError as e:
            print('Transfer amount must be numeric!', e)
            return

        try:
            if self.amt > self.xfer_limit:
                raise XferLimitException('Transfer amount exceeded limit')
            elif self.amt >= self.balance:
                raise XferLimitException('Trying to transfer more than account balance')
        except XferLimitException as e:
            print('Transfer error:', e)
            return

        if len(str(self.amt).split('.')[1]) > 2:
            self.amt = decimal.Decimal(amt).quantize(decimal.Decimal('0.01'))
            print('Transfer amount can only have two decimal places')
            print('Amount has been adjusted to {}'.format(self.amt))

        self.xferfund(self.amt)

    def xferfund(self, xferamt):
        balance = self.balance - float(xferamt)
        self.balance = balance


if __name__ == "__main__":
    accnum = input('Enter your account number: ')
    try:
        int(accnum)
    except ValueError as e:
        print('Account number must be numbers only')
        sys.exit(1)

    if len(accnum) != 9:
        print('Account numbers must be 9 characters long')
        sys.exit(1)

    accnum = myAccount(accnum, 3000.00) # Create account object

    xferamt = input('Enter amount to transfer ') # get amount to transfer

    accnum.checkamt(xferamt) # check transfer amount

    print('Account balance is {0:.2f}'.format(accnum.balance)) # print updated balance


