#pragma once

/**
 * This class provides common functionality used by each of the classes
 * that read the different sections of the MPP file.
 */
class MPPComponent
{
protected:
   /**
    * Constructor.
    */
	MPPComponent();

   /**
    * This method reads a single byte from the input stream.
    *
    * @param is the input stream
    * @return byte value
    * @throws IOException on file read error or EOF
    */
	int readByte(BwProj::toolkit::IDataReadStream* is);

   /**
    * This method reads a two byte integer from the input stream.
    *
    * @param is the input stream
    * @return integer value
    * @throws IOException on file read error or EOF
    */
	int readShort(BwProj::toolkit::IDataReadStream* is);

   /**
    * This method reads a four byte integer from the input stream.
    *
    * @param is the input stream
    * @return byte value
    * @throws IOException on file read error or EOF
    */
	long readInt(BwProj::toolkit::IDataReadStream* is);

   /**
    * This method reads a byte array from the input stream.
    *
    * @param is the input stream
    * @param size number of bytes to read
    * @return byte array
    * @throws IOException on file read error or EOF
    */
	ByteArrayPtr readByteArray2(BwProj::toolkit::IDataReadStream* is, int size);
};