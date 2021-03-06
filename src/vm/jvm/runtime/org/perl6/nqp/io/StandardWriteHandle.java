package org.perl6.nqp.io;

import java.io.IOException;
import java.io.PrintStream;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CharsetEncoder;

import org.perl6.nqp.runtime.ExceptionHandling;
import org.perl6.nqp.runtime.ThreadContext;

public class StandardWriteHandle implements IIOClosable, IIOSeekable, IIOEncodable, IIOSyncWritable {
    private PrintStream ps;
    private CharsetEncoder enc;
    private CharsetDecoder dec;
    private long pos;
    
    public StandardWriteHandle(ThreadContext tc, PrintStream ps) {
        this.ps = ps;
        this.pos = 0;
        setEncoding(tc, Charset.forName("UTF-8"));
    }
    
    public void close(ThreadContext tc) {
        ps.close();
    }
    
    public void seek(ThreadContext tc, long offset, long whence) {
        throw ExceptionHandling.dieInternal(tc, "Cannot seek stdout or stderr");
    }
    
    public long tell(ThreadContext tc) {
        return pos;
    }
    
    public void setEncoding(ThreadContext tc, Charset cs) {
        enc = cs.newEncoder();
        dec = cs.newDecoder();
    }
    
    public void write(ThreadContext tc, byte[] bytes) {
    	ps.write(bytes, 0, bytes.length);
    }
    
    public void print(ThreadContext tc, String s) {
        try {
            ByteBuffer buffer = enc.encode(CharBuffer.wrap(s));
            byte[] bytes = buffer.array();
            ps.write(bytes, 0, buffer.limit());
        } catch (IOException e) {
            throw ExceptionHandling.dieInternal(tc, e);
        }
    }
    
    public void say(ThreadContext tc, String s) {
        print(tc, s);
        print(tc, System.lineSeparator());
    }
    
    public void flush(ThreadContext tc) {
        ps.flush();
    }
}
