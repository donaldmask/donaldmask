// Copyright (c) 2011-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DONALDMASK_QT_DONALDMASKADDRESSVALIDATOR_H
#define DONALDMASK_QT_DONALDMASKADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class DonaldmaskAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit DonaldmaskAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Donaldmask address widget validator, checks for a valid donaldmask address.
 */
class DonaldmaskAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit DonaldmaskAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // DONALDMASK_QT_DONALDMASKADDRESSVALIDATOR_H
