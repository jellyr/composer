#pragma once
#include <QString>
#include <QStack>
#include <QVariant>
#include <QTextStream>
#include <QDataStream>
#include <ostream>

struct Operation
{
	enum OperationFlags { NORMAL = 0, NO_EXEC = 1, NO_EMIT = 2 };

	Operation() { }
	Operation(const QString &opString) { *this << opString; }

	// Functions to add parameters to Operation

	Operation& operator<<(const QString &str) { m_params.push_back(QVariant(str)); return *this; }
	//Operation& operator<<(char c) { m_params.push_back(QVariant(c)); return *this; }
	Operation& operator<<(int i) { m_params.push_back(QVariant(i)); return *this; }
	//Operation& operator<<(unsigned u) { m_params.push_back(QVariant(u)); return *this; }
	Operation& operator<<(bool b) { m_params.push_back(QVariant(b)); return *this; }
	Operation& operator<<(float f) { m_params.push_back(QVariant(f)); return *this; }
	Operation& operator<<(double d) { m_params.push_back(QVariant(d)); return *this; }
	Operation& operator<<(QVariant q) { m_params.push_back(q); return *this; }

	/// Get the operation id
	QString op() const { return m_params.isEmpty() ? "" : m_params.front().toString(); }
	/// Get parameter count (excluding operation id)
	int paramCount() const { return m_params.size() - 1; }

	/// Overloaded template getter for param at certain position (1-based)
	template<typename T>
	T param(int index) const { m_params[index].value<T>(); }

	/// Get Operation parameter at certain index (1-based)

	QString s(int index) const { return m_params[index].toString(); }
	char c(int index) const { return m_params[index].toChar().toAscii(); }
	int i(int index) const { return m_params[index].toInt(); }
	unsigned u(int index) const { return m_params[index].toUInt(); }
	bool b(int index) const { return m_params[index].toBool(); }
	float f(int index) const { return m_params[index].toFloat(); }
	double d(int index) const { return m_params[index].toDouble(); }
	QVariant q(int index) const { return m_params[index]; }

	std::string dump() const {
		QString st;
		QTextStream ts(&st);
		foreach(QVariant qv, m_params)
			ts << qv.toString() << " ";
		return st.toStdString();
	}

	friend QDataStream& operator<<(QDataStream&, const Operation&);
	friend QDataStream& operator>>(QDataStream& stream, Operation& op);

private:
	QList<QVariant> m_params;
};

typedef QStack<Operation> OperationStack;

// Serialization operators
QDataStream& operator<<(QDataStream& stream, const Operation& op);
QDataStream& operator>>(QDataStream& stream, Operation& op);
